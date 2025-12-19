
#pragma once

#include "Sere_vulkan_types.h"

b8 Sere_CreateShaderModule(
    Sere_VulkanContext *context,
    const char *name,
    const char *type_str,
    VkShaderStageFlagBits shader_stage_flag,
    u32 stage_index,
    Sere_VulkanShaderStage *shader_stages
);
