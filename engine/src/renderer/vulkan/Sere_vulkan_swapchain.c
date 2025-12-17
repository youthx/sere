
#include "Sere_vulkan_swapchain.h"
#include "Sere_vulkan_device.h"
#include "Sere_vulkan_image.h"

#include "core/Sere_logger.h"
#include "core/Sere_memory.h"

void _Sere_CreateVulkanSwapchain(Sere_VulkanContext *context, u32 width, u32 height, Sere_VulkanSwapchain *swapchain);
void _Sere_DestroyVulkanSwapchain(Sere_VulkanContext *context, Sere_VulkanSwapchain *swapchain);

Sere_VulkanSwapchain *Sere_CreateVulkanSwapchain(Sere_VulkanContext *context, u32 width, u32 height)
{
    Sere_VulkanSwapchain *swapchain = (Sere_VulkanSwapchain *)Sere_Alloc(sizeof(Sere_VulkanSwapchain), SERE_MEMORY_TAG_RENDERER);
    _Sere_CreateVulkanSwapchain(context, width, height, swapchain);
    return swapchain;
}

void Sere_RecreateVulkanSwapchain(Sere_VulkanContext *context, u32 width, u32 height, Sere_VulkanSwapchain *swapchain)
{
    _Sere_DestroyVulkanSwapchain(context, swapchain);
    _Sere_CreateVulkanSwapchain(context, width, height, swapchain);
}

void Sere_DestroyVulkanSwapchain(Sere_VulkanContext *context, Sere_VulkanSwapchain *swapchain)
{
    _Sere_DestroyVulkanSwapchain(context, swapchain);
}

b8 Sere_VulkanSwapchainAcquireNextImageIndex(
    Sere_VulkanContext *context,
    Sere_VulkanSwapchain *swapchain,
    u64 timeout_ns,
    VkSemaphore image_available_semaphore,
    VkFence fence,
    u32 *out_image_index)
{
    VkResult result = vkAcquireNextImageKHR(
        context->device.logical_device,
        swapchain->handle,
        timeout_ns,
        image_available_semaphore,
        fence,
        out_image_index);

    if (result == VK_ERROR_OUT_OF_DATE_KHR)
    {
        Sere_RecreateVulkanSwapchain(context, context->framebuffer_width, context->framebuffer_height, swapchain);
        return SERE_FALSE;
    }
    else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
    {
        SERE_FATAL("Failed to acquire swapchain image.");
        return SERE_FALSE;
    }
    return SERE_TRUE;
}

void Sere_VulkanSwapchainPresent(
    Sere_VulkanContext *context,
    Sere_VulkanSwapchain *swapchain,
    VkQueue graphics_queue,
    VkQueue present_queue,
    VkSemaphore render_complete_semaphore,
    u32 present_image_index)
{
    VkPresentInfoKHR present_info = {VK_STRUCTURE_TYPE_PRESENT_INFO_KHR};
    present_info.waitSemaphoreCount = 1;
    present_info.pWaitSemaphores = &render_complete_semaphore;
    present_info.swapchainCount = 1;
    present_info.pSwapchains = &swapchain->handle;
    present_info.pImageIndices = &present_image_index;
    present_info.pResults = 0;

    VkResult result = vkQueuePresentKHR(present_queue, &present_info);
    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR)
    {
        Sere_RecreateVulkanSwapchain(context, context->framebuffer_width, context->framebuffer_height, swapchain);
    }
    else if (result != VK_SUCCESS)
    {
        SERE_FATAL("Failed to present swapchain image.");
    }
}

void _Sere_CreateVulkanSwapchain(Sere_VulkanContext *context, u32 width, u32 height, Sere_VulkanSwapchain *swapchain)
{
    VkExtent2D swapchain_extent = {width, height};
    swapchain->max_frames_in_flight = 2;

    b8 found = SERE_FALSE;
    for (u32 i = 0; i < context->device.swapchain_support.format_count; ++i)
    {
        VkSurfaceFormatKHR format = context->device.swapchain_support.formats[i];

        if (format.format == VK_FORMAT_B8G8R8A8_UNORM &&
            format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
        {
            swapchain->image_format = format;
            found = SERE_TRUE;
            break;
        }
    }

    if (!found)
    {
        swapchain->image_format = context->device.swapchain_support.formats[0];
    }

    VkPresentModeKHR present_mode = VK_PRESENT_MODE_FIFO_KHR;
    for (u32 i = 0; i < context->device.swapchain_support.present_mode_count; ++i)
    {
        VkPresentModeKHR mode = context->device.swapchain_support.present_modes[i];
        if (mode == VK_PRESENT_MODE_MAILBOX_KHR)
        {
            present_mode = mode;
            break;
        }
    }

    Sere_VulkanDeviceQuerySwapchainSupport(
        context->device.phyiscal_device,
        context->surface,
        &context->device.swapchain_support);

    if (context->device.swapchain_support.capabilities.currentExtent.width != UINT32_MAX)
    {
        swapchain_extent = context->device.swapchain_support.capabilities.currentExtent;
    }

    VkExtent2D min = context->device.swapchain_support.capabilities.minImageExtent;
    VkExtent2D max = context->device.swapchain_support.capabilities.maxImageExtent;
    swapchain_extent.width = SERE_CLAMP(swapchain_extent.width, min.width, max.width);
    swapchain_extent.height = SERE_CLAMP(swapchain_extent.height, min.height, max.height);

    u32 image_count = context->device.swapchain_support.capabilities.minImageCount + 1;
    if (context->device.swapchain_support.capabilities.maxImageCount > 0 &&
        image_count > context->device.swapchain_support.capabilities.maxImageCount)
    {
        image_count = context->device.swapchain_support.capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR swapchain_create_info = {VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR};
    swapchain_create_info.surface = context->surface;
    swapchain_create_info.minImageCount = image_count;
    swapchain_create_info.imageFormat = swapchain->image_format.format;
    swapchain_create_info.imageColorSpace = swapchain->image_format.colorSpace;
    swapchain_create_info.imageExtent = swapchain_extent;
    swapchain_create_info.imageArrayLayers = 1;
    swapchain_create_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    if (context->device.graphics_queue_index != context->device.present_queue_index)
    {
        u32 queueFamilyIndices[] = {
            (u32)context->device.graphics_queue_index,
            (u32)context->device.present_queue_index};

        swapchain_create_info.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        swapchain_create_info.queueFamilyIndexCount = 2;
        swapchain_create_info.pQueueFamilyIndices = queueFamilyIndices;
    }
    else
    {
        swapchain_create_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        swapchain_create_info.queueFamilyIndexCount = 0;
        swapchain_create_info.pQueueFamilyIndices = 0;
    }

    swapchain_create_info.preTransform = context->device.swapchain_support.capabilities.currentTransform;
    swapchain_create_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    swapchain_create_info.presentMode = present_mode;
    swapchain_create_info.clipped = VK_TRUE;
    swapchain_create_info.oldSwapchain = 0;

    SERE_VK_CHECK(vkCreateSwapchainKHR(context->device.logical_device, &swapchain_create_info, context->allocator, &swapchain->handle));

    context->current_frame = 0;
    swapchain->image_count = 0;
    SERE_VK_CHECK(vkGetSwapchainImagesKHR(context->device.logical_device, swapchain->handle, &swapchain->image_count, 0));
    if (!swapchain->images)
    {
        swapchain->images = (VkImage *)Sere_Alloc(sizeof(VkImage) * swapchain->image_count, SERE_MEMORY_TAG_RENDERER);
    }

    if (!swapchain->views)
    {
        swapchain->views = (VkImageView *)Sere_Alloc(sizeof(VkImageView) * swapchain->image_count, SERE_MEMORY_TAG_RENDERER);
    }

    SERE_VK_CHECK(vkGetSwapchainImagesKHR(context->device.logical_device, swapchain->handle, &swapchain->image_count, swapchain->images));

    for (u32 i = 0; i < swapchain->image_count; ++i)
    {
        VkImageViewCreateInfo view_info = {VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO};
        view_info.image = swapchain->images[i];
        view_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
        view_info.format = swapchain->image_format.format;
        view_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        view_info.subresourceRange.baseMipLevel = 0;
        view_info.subresourceRange.levelCount = 1;
        view_info.subresourceRange.baseArrayLayer = 0;
        view_info.subresourceRange.layerCount = 1;

        SERE_VK_CHECK(vkCreateImageView(context->device.logical_device, &view_info, context->allocator, &swapchain->views[i]));
    }

    if (!Sere_VulkanDeviceDetectDepthFormat(&context->device))
    {
        context->device.depth_format = VK_FORMAT_UNDEFINED;
        SERE_FATAL("Failed to find a supported format!");
    }
    swapchain->depth_attachment = *Sere_CreateVulkanImage(
        context,
        VK_IMAGE_TYPE_2D,
        swapchain_extent.width,
        swapchain_extent.height,
        context->device.depth_format,
        VK_IMAGE_TILING_OPTIMAL,
        VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
        SERE_TRUE,
        VK_IMAGE_ASPECT_DEPTH_BIT);
}

void _Sere_DestroyVulkanSwapchain(Sere_VulkanContext *context, Sere_VulkanSwapchain *swapchain)
{
    Sere_DestroyVulkanImage(context, &swapchain->depth_attachment);

    for (u32 i = 0; i < swapchain->image_count; ++i)
    {
        vkDestroyImageView(context->device.logical_device, swapchain->views[i], context->allocator);
    }

    vkDestroySwapchainKHR(context->device.logical_device, swapchain->handle, context->allocator);
}
