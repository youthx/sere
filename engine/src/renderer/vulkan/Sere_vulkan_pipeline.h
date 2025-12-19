
#pragma once

#include "Sere_vulkan_types.h"

Sere_VulkanPipeline *Sere_CreateVulkanGraphicsPipeline(
    Sere_VulkanContext *context,
    Sere_VulkanRenderPass *renderpass,
    u32 attribute_count,
    VkVertexInputAttributeDescription *attributes,
    u32 descriptor_set_layout_count,
    VkDescriptorSetLayout *descriptor_set_layouts,
    u32 stage_count,
    VkPipelineShaderStageCreateInfo *stages,
    VkViewport viewport,
    VkRect2D scissor,
    b8 is_wireframe);

void Sere_DestroyVulkanGraphicsPipeline(Sere_VulkanContext *context, Sere_VulkanPipeline *pipeline);

void Sere_VulkanGraphicsPipelineBind(Sere_VulkanCommandBuffer *command_buffer, VkPipelineBindPoint bind_point, Sere_VulkanPipeline *pipeline);
