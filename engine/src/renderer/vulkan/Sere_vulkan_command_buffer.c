

#include "Sere_vulkan_command_buffer.h"

#include "core/Sere_memory.h"

void Sere_VulkanCommandBufferAlloc(
    Sere_VulkanContext *context,
    VkCommandPool pool,
    b8 is_primary,
    Sere_VulkanCommandBuffer *out_command_buffer)
{

    Sere_ZeroMemory(out_command_buffer, sizeof(out_command_buffer));

    VkCommandBufferAllocateInfo alloc_info = {VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO};
    alloc_info.commandPool = pool;
    alloc_info.level = is_primary ? VK_COMMAND_BUFFER_LEVEL_PRIMARY : VK_COMMAND_BUFFER_LEVEL_SECONDARY;
    alloc_info.commandBufferCount = 1;
    alloc_info.pNext = 0;

    out_command_buffer->state = SERE_COMMAND_BUFFER_STATE_NOT_ALLOCATED;
    SERE_VK_CHECK(
        vkAllocateCommandBuffers(
            context->device.logical_device,
            &alloc_info,
            &out_command_buffer->handle));

    out_command_buffer->state = SERE_COMMAND_BUFFER_STATE_READY;
}

void Sere_VulkanCommandBufferFree(
    Sere_VulkanContext *context,
    VkCommandPool pool,
    Sere_VulkanCommandBuffer *command_buffer)
{
    vkFreeCommandBuffers(
        context->device.logical_device,
        pool,
        1,
        &command_buffer->handle);

    command_buffer->handle = 0;
    command_buffer->state = SERE_COMMAND_BUFFER_STATE_NOT_ALLOCATED;
}

void Sere_VulkanCommandBufferBegin(
    Sere_VulkanCommandBuffer *command_buffer,
    b8 is_single_use,
    b8 is_renderpass_continue,
    b8 is_simultaneous_use)
{
    VkCommandBufferBeginInfo begin_info = {VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO};
    begin_info.flags = 0;
    if (is_single_use)
    {
        begin_info.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    }
    if (is_renderpass_continue)
    {
        begin_info.flags |= VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT;
    }
    if (is_simultaneous_use)
    {
        begin_info.flags |= VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
    }

    SERE_VK_CHECK(vkBeginCommandBuffer(command_buffer->handle, &begin_info));
    command_buffer->state = SERE_COMMAND_BUFFER_STATE_RECORDING;
}

void Sere_VulkanCommandBufferEnd(Sere_VulkanCommandBuffer *command_buffer)
{
    SERE_VK_CHECK(vkEndCommandBuffer(command_buffer->handle));
    command_buffer->state = SERE_COMMAND_BUFFER_STATE_RECORDING_ENDED;
}

void Sere_VulkanCommandBufferUpdateSubmitted(Sere_VulkanCommandBuffer *command_buffer)
{
    command_buffer->state = SERE_COMMAND_BUFFER_STATE_SUBMITTED;
}

void Sere_VulkanCommandBufferReset(Sere_VulkanCommandBuffer *command_buffer)
{
    command_buffer->state = SERE_COMMAND_BUFFER_STATE_READY;
}

void Sere_VulkanCommandBufferAllocAndBeginSingleUse(
    Sere_VulkanContext *context,
    VkCommandPool pool,
    Sere_VulkanCommandBuffer *out_command_buffer)
{
    Sere_VulkanCommandBufferAlloc(context, pool, SERE_TRUE, out_command_buffer);
    Sere_VulkanCommandBufferBegin(out_command_buffer, SERE_TRUE, SERE_FALSE, SERE_FALSE);
}

void Sere_VulkanCommandBufferEndSingleUse(
    Sere_VulkanContext *context,
    VkCommandPool pool,
    Sere_VulkanCommandBuffer *command_buffer,
    VkQueue queue)
{
    Sere_VulkanCommandBufferEnd(command_buffer);

    VkSubmitInfo submit_info = {VK_STRUCTURE_TYPE_SUBMIT_INFO};
    submit_info.commandBufferCount = 1;
    submit_info.pCommandBuffers = &command_buffer->handle;
    SERE_VK_CHECK(vkQueueSubmit(queue, 1, &submit_info, 0));

    SERE_VK_CHECK(vkQueueWaitIdle(queue));
    
    Sere_VulkanCommandBufferFree(context, pool, command_buffer);
}
