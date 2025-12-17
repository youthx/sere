
#pragma once

#include "Sere_vulkan_types.h"

Sere_VulkanSwapchain *Sere_CreateVulkanSwapchain(Sere_VulkanContext *context, u32 width, u32 height);

void Sere_RecreateVulkanSwapchain(Sere_VulkanContext *context, u32 width, u32 height, Sere_VulkanSwapchain *swapchain);

void Sere_DestroyVulkanSwapchain(Sere_VulkanContext *context, Sere_VulkanSwapchain *swapchain);

b8 Sere_VulkanSwapchainAcquireNextImageIndex(
    Sere_VulkanContext *context,
    Sere_VulkanSwapchain *swapchain,
    u64 timeout_ns,
    VkSemaphore image_available_semaphore,
    VkFence fence,
    u32 *out_image_index);

void Sere_VulkanSwapchainPresent(
    Sere_VulkanContext *context,
    Sere_VulkanSwapchain *swapchain,
    VkQueue graphics_queue,
    VkQueue present_queue,
    VkSemaphore render_complete_semaphore,
    u32 present_image_index);

