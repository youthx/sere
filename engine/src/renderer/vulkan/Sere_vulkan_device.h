
#pragma once

#include "Sere_vulkan_types.h"

b8 Sere_CreateVulkanDevice(Sere_VulkanContext *context);

void Sere_DestroyVulkanDevice(Sere_VulkanContext *context);

void Sere_VulkanDeviceQuerySwapchainSupport(
    VkPhysicalDevice physical_device,
    VkSurfaceKHR surface,
    Sere_VulkanSwapchainSupportInfo *out_support_info);

b8 Sere_VulkanDeviceDetectDepthFormat(Sere_VulkanDevice *device);