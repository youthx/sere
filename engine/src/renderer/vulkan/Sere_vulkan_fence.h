
#pragma once

#include "Sere_vulkan_types.h"

Sere_VulkanFence *Sere_CreateVulkanFence(
    Sere_VulkanContext *context,
    b8 create_signaled);

void Sere_DestroyVulkanFence(Sere_VulkanContext *context, Sere_VulkanFence *fence);

b8 Sere_VulkanFenceWait(Sere_VulkanContext *context, Sere_VulkanFence *fence, u64 timeout_ns);

void Sere_VulkanFenceReset(Sere_VulkanContext *context, Sere_VulkanFence *fence);
