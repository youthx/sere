
#pragma once

#include "Sere_vulkan_types.h"

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
    VkImageAspectFlags view_aspect_flags);

void Sere_CreateVulkanImageView(
    Sere_VulkanContext *context,
    VkFormat format,
    Sere_VulkanImage *image,
    VkImageAspectFlags aspect_flags);

void Sere_DestroyVulkanImage(
    Sere_VulkanContext *context,
    Sere_VulkanImage *image);
