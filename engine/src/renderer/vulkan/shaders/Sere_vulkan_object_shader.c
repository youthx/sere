
#include "Sere_vulkan_object_shader.h"
#include "core/Sere_memory.h"
#include "core/Sere_logger.h"

#include "renderer/vulkan/Sere_vulkan_shader_utils.h"

#define SERE_BUILTIN_SHADER_NAME_OBJECT "Builtin.ObjectShader"

Sere_VulkanObjectShader *Sere_CreateVulkanObjectShader(Sere_VulkanContext *context)
{
    Sere_VulkanObjectShader *shader = (Sere_VulkanObjectShader *)Sere_Alloc(sizeof(Sere_VulkanObjectShader), SERE_MEMORY_TAG_RENDERER);

    char stage_type_strs[SERE_OBJECT_SHADER_STAGE_COUNT][5] = {"vert", "frag"};
    VkShaderStageFlagBits stage_types[SERE_OBJECT_SHADER_STAGE_COUNT] = {VK_SHADER_STAGE_VERTEX_BIT, VK_SHADER_STAGE_FRAGMENT_BIT};

    for (u32 i = 0; i < SERE_OBJECT_SHADER_STAGE_COUNT; ++i) {
        if (!Sere_CreateShaderModule(context, SERE_BUILTIN_SHADER_NAME_OBJECT, stage_type_strs[i], stage_types[i], i, shader->stages)) {
            SERE_ERROR("Unable to create %s shader module for '%s'.", stage_type_strs[i], SERE_BUILTIN_SHADER_NAME_OBJECT);
            return NULL;
        }
    }

    return shader;
}

void Sere_DestroyVulkanObjectShader(Sere_VulkanContext *context, Sere_VulkanObjectShader *shader)
{
    for (u32 i = 0; i < SERE_OBJECT_SHADER_STAGE_COUNT; ++i) {
        vkDestroyShaderModule(context->device.logical_device, shader->stages[i].handle, context->allocator);
        shader->stages[i].handle = 0;
    }
}

void Sere_VulkanObjectShaderUse(Sere_VulkanContext *context, Sere_VulkanObjectShader *shader)
{
}