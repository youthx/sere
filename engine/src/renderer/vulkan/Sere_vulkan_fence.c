
#include "Sere_vulkan_fence.h"

#include "core/Sere_memory.h"
#include "core/Sere_logger.h"

Sere_VulkanFence *Sere_CreateVulkanFence(
    Sere_VulkanContext *context,
    b8 create_signaled)
{
    Sere_VulkanFence *out_fence = (Sere_VulkanFence *)Sere_Alloc(sizeof(Sere_VulkanFence *), SERE_MEMORY_TAG_RENDERER);

    out_fence->is_signaled = create_signaled;

    VkFenceCreateInfo create_info = {VK_STRUCTURE_TYPE_FENCE_CREATE_INFO};
    if (out_fence->is_signaled)
    {
        create_info.flags = VK_FENCE_CREATE_SIGNALED_BIT;
    }

    SERE_VK_CHECK(
        vkCreateFence(
            context->device.logical_device,
            &create_info,
            context->allocator,
            &out_fence->handle));
    return out_fence;
}

void Sere_DestroyVulkanFence(Sere_VulkanContext *context, Sere_VulkanFence *fence)
{
    if (fence->handle)
    {
        vkDestroyFence(context->device.logical_device, fence->handle,
                       context->allocator);
        fence->handle = 0;
    }

    fence->is_signaled = SERE_FALSE;
}

b8 Sere_VulkanFenceWait(Sere_VulkanContext *context, Sere_VulkanFence *fence, u64 timeout_ns)
{
    if (!fence->is_signaled)
    {
        VkResult result = vkWaitForFences(
            context->device.logical_device,
            1,
            &fence->handle,
            SERE_TRUE,
            timeout_ns);
        switch (result)
        {
        case VK_SUCCESS:
            fence->is_signaled = SERE_TRUE;
            break;
        default:
            SERE_ERROR("Sere_VulkanFenceWait - An error occurred.");
            break;
        }
    }
    else
    {
        return SERE_TRUE;
    }
    return SERE_FALSE;
}

void Sere_VulkanFenceReset(Sere_VulkanContext *context, Sere_VulkanFence *fence)
{
    if (fence->is_signaled)
    {
        SERE_VK_CHECK(vkResetFences(context->device.logical_device, 1, &fence->handle));
        fence->is_signaled = SERE_FALSE;
    }
}
