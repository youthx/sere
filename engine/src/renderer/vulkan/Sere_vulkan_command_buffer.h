
#pragma once

#include "Sere_vulkan_types.h"

void Sere_VulkanCommandBufferAlloc(
    Sere_VulkanContext *context,
    VkCommandPool pool,
    b8 is_primary,
    Sere_VulkanCommandBuffer *out_command_buffer);

void Sere_VulkanCommandBufferFree(
    Sere_VulkanContext *context,
    VkCommandPool pool,
    Sere_VulkanCommandBuffer *command_buffer);

void Sere_VulkanCommandBufferBegin(
    Sere_VulkanCommandBuffer *command_buffer,
    b8 is_single_use,
    b8 is_renderpass_continue,
    b8 is_simultaneous_use);

void Sere_VulkanCommandBufferEnd(Sere_VulkanCommandBuffer *command_buffer);

void Sere_VulkanCommandBufferUpdateSubmitted(Sere_VulkanCommandBuffer *command_buffer);

void Sere_VulkanCommandBufferReset(Sere_VulkanCommandBuffer *command_buffer);

void Sere_VulkanCommandBufferAllocAndBeginSingleUse(
    Sere_VulkanContext *context,
    VkCommandPool pool,
    Sere_VulkanCommandBuffer *out_command_buffer);

void Sere_VulkanCommandBufferEndSingleUse(
    Sere_VulkanContext *context,
    VkCommandPool pool,
    Sere_VulkanCommandBuffer *command_buffer,
    VkQueue queue);
    