
#pragma once

#include "Sere_vulkan_types.h"

Sere_VulkanBuffer *Sere_CreateVulkanBuffer(
    Sere_VulkanContext *context,
    u64 size,
    VkBufferUsageFlagBits usage,
    u32 memory_property_flags,
    b8 bind_on_create);

void Sere_DestroyVulkanBuffer(Sere_VulkanContext *context, Sere_VulkanBuffer *buffer);

b8 Sere_VulkanBufferResize(
    Sere_VulkanContext *context,
    u64 new_size,
    Sere_VulkanBuffer *buffer,
    VkQueue queue,
    VkCommandPool pool);


void Sere_VulkanBufferBind(Sere_VulkanContext *context, Sere_VulkanBuffer *buffer, u64 offset);

void *Sere_VulkanBufferLockMemory(Sere_VulkanContext *context, Sere_VulkanBuffer *buffer, u64 offset, u64 size, u32 flags);
void Sere_VulkanBufferUnlockMemory(Sere_VulkanContext *context, Sere_VulkanBuffer *buffer);

void Sere_VulkanBufferLoadData(Sere_VulkanContext *context, Sere_VulkanBuffer *buffer, u64 offset, u64 size, u32 flags, const void *data);

void Sere_VulkanBufferCopyTo(
    Sere_VulkanContext *context,
    VkCommandPool pool,
    VkFence fence,
    VkQueue queue,
    VkBuffer source,
    u64 source_offset,
    VkBuffer dest,
    u64 dest_offset,
    u64 size);