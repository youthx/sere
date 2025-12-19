
#include "Sere_vulkan_object_shader.h"

#include "core/Sere_memory.h"
#include "core/Sere_logger.h"

#include "renderer/vulkan/Sere_vulkan_shader_utils.h"
#include "renderer/vulkan/Sere_vulkan_pipeline.h"

#include "math/Sere_math_types.h"

#define SERE_BUILTIN_SHADER_NAME_OBJECT "Builtin.ObjectShader"

Sere_VulkanObjectShader *Sere_CreateVulkanObjectShader(Sere_VulkanContext *context)
{
    Sere_VulkanObjectShader *shader = (Sere_VulkanObjectShader *)Sere_Alloc(sizeof(Sere_VulkanObjectShader), SERE_MEMORY_TAG_RENDERER);

    char stage_type_strs[SERE_OBJECT_SHADER_STAGE_COUNT][5] = {"vert", "frag"};
    VkShaderStageFlagBits stage_types[SERE_OBJECT_SHADER_STAGE_COUNT] = {VK_SHADER_STAGE_VERTEX_BIT, VK_SHADER_STAGE_FRAGMENT_BIT};

    for (u32 i = 0; i < SERE_OBJECT_SHADER_STAGE_COUNT; ++i)
    {
        if (!Sere_CreateShaderModule(context, SERE_BUILTIN_SHADER_NAME_OBJECT, stage_type_strs[i], stage_types[i], i, shader->stages))
        {
            SERE_ERROR("Unable to create %s shader module for '%s'.", stage_type_strs[i], SERE_BUILTIN_SHADER_NAME_OBJECT);
            return NULL;
        }
    }

    VkViewport viewport;
    viewport.x = 0.0f;
    viewport.y = (f32)context->framebuffer_height;
    viewport.width = (f32)context->framebuffer_width;
    viewport.height = -(f32)context->framebuffer_height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;

    VkRect2D scissor;
    scissor.offset.x = scissor.offset.y = 0;
    scissor.extent.width = context->framebuffer_width;
    scissor.extent.height = context->framebuffer_height;

    u32 offset = 0;
    const i32 attribute_count = 1;
    VkVertexInputAttributeDescription attribute_descriptions[attribute_count];

    VkFormat formats[1] = {
        VK_FORMAT_R32G32B32_SFLOAT,
    };

    u64 sizes[1] = {
        sizeof(Sere_Vec3),
    };

    for (u32 i = 0; i < attribute_count; ++i)
    {
        attribute_descriptions[i].binding = 0;
        attribute_descriptions[i].location = i;
        attribute_descriptions[i].format = formats[i];
        attribute_descriptions[i].offset = offset;
        offset += sizes[i];
    }

    VkPipelineShaderStageCreateInfo stage_create_infos[SERE_OBJECT_SHADER_STAGE_COUNT];
    Sere_ZeroMemory(stage_create_infos, sizeof(stage_create_infos));
    for (u32 i = 0; i < SERE_OBJECT_SHADER_STAGE_COUNT; ++i)
    {
        stage_create_infos[i].sType = shader->stages[i].shader_stage_create_info.sType;
        stage_create_infos[i] = shader->stages[i].shader_stage_create_info;
    }

    shader->pipeline = *Sere_CreateVulkanGraphicsPipeline(
        context,
        &context->main_renderpass,
        attribute_count,
        attribute_descriptions,
        0,
        0,
        SERE_OBJECT_SHADER_STAGE_COUNT,
        stage_create_infos,
        viewport,
        scissor,
        SERE_FALSE);
    return shader;
}

void Sere_DestroyVulkanObjectShader(Sere_VulkanContext *context, Sere_VulkanObjectShader *shader)
{

    Sere_DestroyVulkanGraphicsPipeline(context, &shader->pipeline);
    for (u32 i = 0; i < SERE_OBJECT_SHADER_STAGE_COUNT; ++i)
    {
        vkDestroyShaderModule(context->device.logical_device, shader->stages[i].handle, context->allocator);
        shader->stages[i].handle = 0;
    }
}

void Sere_VulkanObjectShaderUse(Sere_VulkanContext *context, Sere_VulkanObjectShader *shader)
{
}