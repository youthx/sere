
#pragma once

#include "Sere_defines.h"
#include "core/Sere_asserts.h"

#include <vulkan/vulkan.h>

#define SERE_VK_CHECK(expr)              \
    {                                    \
        SERE_ASSERT(expr == VK_SUCCESS); \
    }
    
typedef struct Sere_VulkanContext
{
    VkInstance instance;
    VkAllocationCallbacks *allocator;

#if defined(_DEBUG)
    VkDebugUtilsMessengerEXT debug_messenger;
#endif 

} Sere_VulkanContext;
