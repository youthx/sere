
#pragma once

#include "Sere_vulkan_types.h"

const char *Sere_VulkanResultString(VkResult result, b8 get_extended);

b8 Sere_VulkanResultIsSuccess(VkResult result);
