
#pragma once

#include "Sere_defines.h"
#include "core/Sere_asserts.h"

#include <vulkan/vulkan.h>

#define SERE_VK_CHECK(expr)              \
    {                                    \
        SERE_ASSERT(expr == VK_SUCCESS); \
    }

typedef struct Sere_VulkanSwapchainSupportInfo
{
    VkSurfaceCapabilitiesKHR capabilities;
    u32 format_count;
    VkSurfaceFormatKHR *formats;
    u32 present_mode_count;
    VkPresentModeKHR *present_modes;
} Sere_VulkanSwapchainSupportInfo;

typedef struct Sere_VulkanDevice
{
    VkPhysicalDevice phyiscal_device;
    VkDevice logical_device;
    Sere_VulkanSwapchainSupportInfo swapchain_support;

    i32 graphics_queue_index;
    i32 present_queue_index;
    i32 transfer_queue_index;

    VkQueue graphics_queue;
    VkQueue present_queue;
    VkQueue transfer_queue;

    VkCommandPool graphics_command_pool;

    VkPhysicalDeviceProperties properties;
    VkPhysicalDeviceFeatures features;
    VkPhysicalDeviceMemoryProperties memory;

    VkFormat depth_format;
} Sere_VulkanDevice;

typedef struct Sere_VulkanImage
{
    VkImage handle;
    VkDeviceMemory memory;
    VkImageView view;
    u32 width;
    u32 height;
} Sere_VulkanImage;

typedef enum Sere_VulkanRenderPassState
{
    SERE_RENDERPASS_READY,
    SERE_RENDERPASS_RECORDING,
    SERE_RENDERPASS_IN_RENDER_PASS,
    SERE_RENDERPASS_RECORDING_ENDED,
    SERE_RENDERPASS_SUBMITTED,
    SERE_RENDERPASS_NOT_ALLOCATED
} Sere_VulkanRenderPassState;

typedef struct Sere_VulkanRenderPass
{
    VkRenderPass handle;
    f32 x, y, w, h;
    f32 r, g, b, a;

    f32 depth;
    u32 stencil;
} Sere_VulkanRenderPass;

typedef struct Sere_VulkanFramebuffer
{
    VkFramebuffer handle;
    u32 attachment_count;
    VkImageView *attachments;
    Sere_VulkanRenderPass *renderpass;
} Sere_VulkanFramebuffer;

typedef struct Sere_VulkanSwapchain
{
    VkSurfaceFormatKHR image_format;
    u8 max_frames_in_flight;

    VkSwapchainKHR handle;
    u32 image_count;
    VkImage *images;
    VkImageView *views;

    Sere_VulkanImage depth_attachment;
    Sere_VulkanFramebuffer *framebuffers; // For on-screen rendering
} Sere_VulkanSwapchain;

typedef enum Sere_VulkanCommandBufferState
{
    SERE_COMMAND_BUFFER_STATE_READY,
    SERE_COMMAND_BUFFER_STATE_RECORDING,
    SERE_COMMAND_BUFFER_STATE_IN_RENDER_PASS,
    SERE_COMMAND_BUFFER_STATE_RECORDING_ENDED,
    SERE_COMMAND_BUFFER_STATE_SUBMITTED,
    SERE_COMMAND_BUFFER_STATE_NOT_ALLOCATED
} Sere_VulkanCommandBufferState;

typedef struct Sere_VulkanCommandBuffer
{
    VkCommandBuffer handle;

    Sere_VulkanCommandBufferState state;
} Sere_VulkanCommandBuffer;

typedef struct Sere_VulkanFence {
    VkFence handle;
    b8 is_signaled;
} Sere_VulkanFence;

typedef struct Sere_VulkanContext
{
    u32 framebuffer_width;
    u32 framebuffer_height;

    u32 image_index;
    u32 current_frame;

    b8 recreating_swapchain;

    VkInstance instance;
    VkAllocationCallbacks *allocator;
    VkSurfaceKHR surface;

#if defined(_DEBUG)
    VkDebugUtilsMessengerEXT debug_messenger;
#endif

    Sere_VulkanCommandBuffer *graphics_command_buffers;

    VkSemaphore *image_available_semaphores;
    VkSemaphore *queue_complete_semaphores;

    u32 in_flight_fence_count;
    Sere_VulkanFence *in_flight_fences;
    Sere_VulkanFence **images_in_flight;

    Sere_VulkanDevice device;
    Sere_VulkanSwapchain swapchain;
    Sere_VulkanRenderPass main_renderpass;

    i32 (*find_memory_index)(u32 type_filter, u32 property_flags);

} Sere_VulkanContext;
