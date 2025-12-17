
#include "Sere_vulkan_image.h"
#include "Sere_vulkan_device.h"

#include "core/Sere_memory.h"
#include "core/Sere_logger.h"

Sere_VulkanImage *Sere_CreateVulkanImage(
    Sere_VulkanContext *context,
    VkImageType image_type,
    u32 width,
    u32 height,
    VkFormat format,
    VkImageTiling tiling,
    VkImageUsageFlags usage,
    VkMemoryPropertyFlags memory_flags,
    b32 create_view,
    VkImageAspectFlags view_aspect_flags)
{
    Sere_VulkanImage *out_image = (Sere_VulkanImage *)Sere_Alloc(sizeof(Sere_VulkanImage), SERE_MEMORY_TAG_RENDERER);
    out_image->width = width;
    out_image->height = height;

    VkImageCreateInfo image_info = {VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO};
    image_info.imageType = VK_IMAGE_TYPE_2D;
    image_info.extent.width = width;
    image_info.extent.height = height;
    image_info.extent.depth = 1;
    image_info.mipLevels = 4;
    image_info.arrayLayers = 1;
    image_info.format = format;
    image_info.tiling = tiling;
    image_info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    image_info.usage = usage;
    image_info.samples = VK_SAMPLE_COUNT_1_BIT;
    image_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    SERE_VK_CHECK(vkCreateImage(context->device.logical_device, &image_info, context->allocator, &out_image->handle));

    VkMemoryRequirements memory_reqs;
    vkGetImageMemoryRequirements(context->device.logical_device, out_image->handle, &memory_reqs);
    i32 memory_type = context->find_memory_index(memory_reqs.memoryTypeBits, memory_flags);
    if (memory_type == -1)
    {
        SERE_ERROR("Required memory type not found. Image not valid.");
    }

    VkMemoryAllocateInfo malloc_info = {VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO};
    malloc_info.allocationSize = memory_reqs.size;
    malloc_info.memoryTypeIndex = memory_type;
    SERE_VK_CHECK(vkAllocateMemory(context->device.logical_device, &malloc_info, context->allocator, &out_image->memory));
    SERE_VK_CHECK(vkBindImageMemory(context->device.logical_device, out_image->handle, out_image->memory, 0));

    if (create_view)
    {
        out_image->view = 0;
        Sere_CreateVulkanImageView(context, format, out_image, view_aspect_flags);
    }
    return out_image;
}

void Sere_CreateVulkanImageView(
    Sere_VulkanContext *context,
    VkFormat format,
    Sere_VulkanImage *image,
    VkImageAspectFlags aspect_flags)
{
    VkImageViewCreateInfo view_info = {VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO};
    view_info.image = image->handle;
    view_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
    view_info.format = format;
    view_info.subresourceRange.aspectMask = aspect_flags;

    view_info.subresourceRange.baseMipLevel = 0;
    view_info.subresourceRange.levelCount = 1;
    view_info.subresourceRange.baseArrayLayer = 0;
    view_info.subresourceRange.layerCount = 1;

    SERE_VK_CHECK(vkCreateImageView(context->device.logical_device, &view_info, context->allocator, &image->view));
}

void Sere_DestroyVulkanImage(
    Sere_VulkanContext *context,
    Sere_VulkanImage *image)
{
    if (image->view) {
        vkDestroyImageView(context->device.logical_device, image->view, context->allocator);
    }
    
    if (image->memory) {
        vkFreeMemory(context->device.logical_device, image->memory, context->allocator);
    }
    
    if (image->handle) {
        vkDestroyImage(context->device.logical_device, image->handle, context->allocator);
    }

    image->view = VK_NULL_HANDLE;
    image->memory = VK_NULL_HANDLE;
    image->handle = VK_NULL_HANDLE;
}

