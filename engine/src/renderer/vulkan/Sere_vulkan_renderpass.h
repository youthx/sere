
#pragma once

#include "Sere_vulkan_types.h"

Sere_VulkanRenderPass *Sere_CreateVulkanRenderPass(
    Sere_VulkanContext *context,
    f32 x, f32 y, f32 w, f32 h,
    f32 r, f32 g, f32 b, f32 a,
    f32 depth,
    u32 stencil);

void Sere_DestroyRenderPass(
    Sere_VulkanContext *context,
    Sere_VulkanRenderPass *renderpass);

void Sere_VulkanRenderPassBegin(
    Sere_VulkanCommandBuffer *command_buffer,
    Sere_VulkanRenderPass *renderpass,
    VkFramebuffer framebuffer);

void Sere_VulkanRenderPassEnd(
    Sere_VulkanCommandBuffer *command_buffer,
    Sere_VulkanRenderPass *renderpass);

    