
#pragma once

#include "Sere_defines.h"

#include <vulkan/vulkan.h>

typedef struct Sere_VulkanContext
{
    VkInstance instance;
    VkAllocationCallbacks *allocator;
} Sere_VulkanContext;
