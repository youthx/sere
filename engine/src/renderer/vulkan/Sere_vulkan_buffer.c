
#include "Sere_vulkan_buffer.h"
#include "Sere_vulkan_device.h"
#include "Sere_vulkan_command_buffer.h"
#include "Sere_vulkan_utils.h"

#include "core/Sere_logger.h"
#include "core/Sere_memory.h"

#pragma once

#include "Sere_vulkan_types.h"

Sere_VulkanBuffer *Sere_CreateVulkanBuffer(
    Sere_VulkanContext *context,
    u64 size,
    VkBufferUsageFlagBits usage,
    u32 memory_property_flags,
    b8 bind_on_create)
{
    Sere_VulkanBuffer *out_buffer = (Sere_VulkanBuffer *)Sere_Alloc(sizeof(Sere_VulkanBuffer), SERE_MEMORY_TAG_RENDERER);
    Sere_ZeroMemory(out_buffer, sizeof(Sere_VulkanBuffer));

    out_buffer->total_size = size;
    out_buffer->usage = usage;
    out_buffer->memory_property_flags = memory_property_flags;

    VkBufferCreateInfo buffer_info = {VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO};
    buffer_info.size = size;
    buffer_info.usage = usage;
    buffer_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    SERE_VK_CHECK(vkCreateBuffer(context->device.logical_device, &buffer_info, context->allocator, &out_buffer->handle));

    VkMemoryRequirements requirements;
    vkGetBufferMemoryRequirements(context->device.logical_device, out_buffer->handle, &requirements);
    out_buffer->memory_index = context->find_memory_index(requirements.memoryTypeBits, out_buffer->memory_property_flags);
    if (out_buffer->memory_index == -1)
    {
        SERE_ERROR("Unable to create vulkan buffer because the required memory type index was not found.");
        return NULL;
    }

    VkMemoryAllocateInfo allocate_info = {VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO};
    allocate_info.allocationSize = requirements.size;
    allocate_info.memoryTypeIndex = (u32)out_buffer->memory_index;

    VkResult result = vkAllocateMemory(
        context->device.logical_device,
        &allocate_info,
        context->allocator,
        &out_buffer->memory);

    if (result != VK_SUCCESS)
    {
        SERE_ERROR("Unable to create vulkan buffer because the required memory allocation failed. Error: %i", result);
    }

    if (bind_on_create)
    {
        Sere_VulkanBufferBind(context, out_buffer, 0);
    }

    return out_buffer;
}

void Sere_DestroyVulkanBuffer(Sere_VulkanContext *context, Sere_VulkanBuffer *buffer)
{
    if (buffer->memory)
    {
        vkFreeMemory(context->device.logical_device, buffer->memory, context->allocator);
        buffer->memory = 0;
    }
    if (buffer->handle)
    {
        vkDestroyBuffer(context->device.logical_device, buffer->handle, context->allocator);
        buffer->handle = 0;
    }

    buffer->total_size = 0;
    buffer->usage = 0;
    buffer->is_locked = SERE_FALSE;
}

b8 Sere_VulkanBufferResize(
    Sere_VulkanContext *context,
    u64 new_size,
    Sere_VulkanBuffer *buffer,
    VkQueue queue,
    VkCommandPool pool)
{
    VkBufferCreateInfo buffer_info = {VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO};
    buffer_info.size = new_size;
    buffer_info.usage = buffer->usage;
    buffer_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    VkBuffer new_buffer;
    SERE_VK_CHECK(vkCreateBuffer(context->device.logical_device, &buffer_info, context->allocator, &new_buffer));

    VkMemoryRequirements requirements;
    vkGetBufferMemoryRequirements(context->device.logical_device, new_buffer, &requirements);

    VkMemoryAllocateInfo allocate_info = {VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO};
    allocate_info.allocationSize = requirements.size;
    allocate_info.memoryTypeIndex = (u32)buffer->memory_index;

    VkDeviceMemory new_memory;
    VkResult result = vkAllocateMemory(context->device.logical_device, &allocate_info, context->allocator, &new_memory);
    if (result != VK_SUCCESS)
    {
        SERE_ERROR("Unable to resize vulkan buffer because the required memory allocation failed. Error: %i", result);
        return SERE_FALSE;
    }

    SERE_VK_CHECK(vkBindBufferMemory(context->device.logical_device, new_buffer, new_memory, 0));

    Sere_VulkanBufferCopyTo(context, pool, 0, queue, buffer->handle, 0, new_buffer, 0, buffer->total_size);

    vkDeviceWaitIdle(context->device.logical_device);

    if (buffer->memory)
    {
        vkFreeMemory(context->device.logical_device, buffer->memory, context->allocator);
        buffer->memory = 0;
    }
    if (buffer->handle)
    {
        vkDestroyBuffer(context->device.logical_device, buffer->handle, context->allocator);
        buffer->handle = 0;
    }

    buffer->total_size = new_size;
    buffer->memory = new_memory;
    buffer->handle = new_buffer;

    return SERE_TRUE;
}

void Sere_VulkanBufferBind(Sere_VulkanContext *context, Sere_VulkanBuffer *buffer, u64 offset)
{
    SERE_VK_CHECK(vkBindBufferMemory(context->device.logical_device, buffer->handle, buffer->memory, offset));
}

void *Sere_VulkanBufferLockMemory(Sere_VulkanContext *context, Sere_VulkanBuffer *buffer, u64 offset, u64 size, u32 flags)
{
    void *data;
    SERE_VK_CHECK(vkMapMemory(context->device.logical_device, buffer->memory, offset, size, flags, &data));
    return data;
}

void Sere_VulkanBufferUnlockMemory(Sere_VulkanContext *context, Sere_VulkanBuffer *buffer)
{
    vkUnmapMemory(context->device.logical_device, buffer->memory);
}

void Sere_VulkanBufferLoadData(Sere_VulkanContext *context, Sere_VulkanBuffer *buffer, u64 offset, u64 size, u32 flags, const void *data)
{
    void *data_ptr;
    SERE_VK_CHECK(vkMapMemory(context->device.logical_device, buffer->memory, offset, size, flags, &data_ptr));
    Sere_CopyMemory(data_ptr, data, size);
    vkUnmapMemory(context->device.logical_device, buffer->memory);
}

void Sere_VulkanBufferCopyTo(
    Sere_VulkanContext *context,
    VkCommandPool pool,
    VkFence fence,
    VkQueue queue,
    VkBuffer source,
    u64 source_offset,
    VkBuffer dest,
    u64 dest_offset,
    u64 size)
{
    vkQueueWaitIdle(queue);

    Sere_VulkanCommandBuffer temp_command_buffer;
    Sere_VulkanCommandBufferAllocAndBeginSingleUse(context, pool, &temp_command_buffer);

    VkBufferCopy copy_region;
    copy_region.srcOffset = source_offset;
    copy_region.dstOffset = dest_offset;
    copy_region.size = size;

    vkCmdCopyBuffer(temp_command_buffer.handle, source, dest, 1, &copy_region);

    Sere_VulkanCommandBufferEndSingleUse(context, pool, &temp_command_buffer, queue);
}