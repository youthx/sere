
#pragma once 

#include "renderer/vulkan/Sere_vulkan_types.h"
#include "renderer/Sere_renderer_types.h"

Sere_VulkanObjectShader *Sere_CreateVulkanObjectShader(Sere_VulkanContext *context);

void Sere_DestroyVulkanObjectShader(Sere_VulkanContext *context, Sere_VulkanObjectShader *shader);

void Sere_VulkanObjectShaderUse(Sere_VulkanContext *context, Sere_VulkanObjectShader *shader);
