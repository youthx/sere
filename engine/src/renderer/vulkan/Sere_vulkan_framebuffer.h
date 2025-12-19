
#pragma once

#include "Sere_vulkan_types.h"

Sere_VulkanFramebuffer *Sere_CreateVulkanFramebuffer(
    Sere_VulkanContext *context,
    Sere_VulkanRenderPass *renderpass,
    u32 width, u32 height,
    u32 attachment_count,
    VkImageView *attachments);

void Sere_DestroyVulkanFramebuffer(
    Sere_VulkanContext *context,
    Sere_VulkanFramebuffer *framebuffer);

    