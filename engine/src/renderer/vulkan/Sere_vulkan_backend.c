#include "Sere_vulkan_backend.h"
#include "Sere_vulkan_types.h"
#include "Sere_vulkan_platform.h"
#include "Sere_vulkan_device.h"
#include "Sere_vulkan_swapchain.h"
#include "Sere_vulkan_renderpass.h"
#include "Sere_vulkan_command_buffer.h"
#include "Sere_vulkan_framebuffer.h"
#include "Sere_vulkan_fence.h"
#include "Sere_vulkan_utils.h"

#include "core/Sere_logger.h"
#include "core/Sere_string.h"
#include "core/Sere_memory.h"
#include "core/Sere_app.h"

#include "containers/Sere_array.h"

#include "shaders/Sere_vulkan_object_shader.h"

static Sere_VulkanContext context;
static u32 cached_framebuffer_width = 0;
static u32 cached_framebuffer_height = 0;

VKAPI_ATTR VkBool32 VKAPI_CALL Sere_VulkanDebugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
    VkDebugUtilsMessageTypeFlagsEXT message_types,
    const VkDebugUtilsMessengerCallbackDataEXT *callback_data,
    void *user_data);

i32 Sere_FindMemoryIndex(u32 type_filter, u32 property_flags);

void Sere_CreateCommandBuffers(Sere_RendererBackend *backend);

void Sere_RegenerateFramebuffers(Sere_RendererBackend *backend, Sere_VulkanSwapchain *swapchain, Sere_VulkanRenderPass *renderpass);

b8 Sere_RecreateSwapchain(Sere_RendererBackend *backend);
b8 Sere_InitVulkanRendererBackend(Sere_RendererBackend *backend,
                                  const char *app_name,
                                  struct Sere_PlatformState *plat_state)
{
    SERE_DEBUG("============================================================");
    SERE_DEBUG(" Vulkan Renderer Backend Initialization");
    SERE_DEBUG("------------------------------------------------------------");
    SERE_DEBUG(" Application : %s", app_name);
    SERE_DEBUG(" Engine      : Sere Engine v1.0.0");
    SERE_DEBUG("============================================================");

    context.find_memory_index = Sere_FindMemoryIndex;
    context.allocator = 0;

    Sere_AppGetFramebufferSize(&cached_framebuffer_width, &cached_framebuffer_height);
    context.framebuffer_width = (cached_framebuffer_width != 0) ? cached_framebuffer_width : 640;
    context.framebuffer_height = (cached_framebuffer_height != 0) ? cached_framebuffer_height : 400;
    cached_framebuffer_width = 0;
    cached_framebuffer_height = 0;

    VkApplicationInfo app_info = {VK_STRUCTURE_TYPE_APPLICATION_INFO};
    app_info.apiVersion = VK_API_VERSION_1_2;
    app_info.pApplicationName = app_name;
    app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    app_info.pEngineName = "Sere Engine";
    app_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);

    VkInstanceCreateInfo create_info = {VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO};
    create_info.pApplicationInfo = &app_info;

    // Required extensions
    const char **required_extensions = Sere_CreateArray(const char *);
    Sere_ArrayPush(required_extensions, &VK_KHR_SURFACE_EXTENSION_NAME);
    Sere_PlatformGetRequiredExtensionNames(&required_extensions);

#if defined(_DEBUG)
    Sere_ArrayPush(required_extensions, &VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

    SERE_DEBUG(" Required Vulkan extensions:");
    u32 ext_count = Sere_ArrayLength(required_extensions);
    for (u32 i = 0; i < ext_count; i++)
    {
        SERE_DEBUG("   - %s", required_extensions[i]);
    }
#endif

    create_info.enabledExtensionCount = Sere_ArrayLength(required_extensions);
    create_info.ppEnabledExtensionNames = required_extensions;

    // Validation layers
    const char **required_validation_layer_names = 0;
    u32 required_validation_layer_count = 0;

#if defined(_DEBUG)
    SERE_DEBUG(" Validation layers enabled. Enumerating available layers...");

    required_validation_layer_names = Sere_CreateArray(const char *);
    Sere_ArrayPush(required_validation_layer_names, &"VK_LAYER_KHRONOS_validation");
    required_validation_layer_count = Sere_ArrayLength(required_validation_layer_names);

    u32 available_layer_count = 0;
    SERE_VK_CHECK(vkEnumerateInstanceLayerProperties(&available_layer_count, 0));
    VkLayerProperties *available_layers =
        Sere_ReserveArray(VkLayerProperties, available_layer_count);
    SERE_VK_CHECK(vkEnumerateInstanceLayerProperties(&available_layer_count, available_layers));

    for (u32 i = 0; i < required_validation_layer_count; ++i)
    {
        const char *layer_name = required_validation_layer_names[i];
        SERE_DEBUG("   Checking for layer: %s", layer_name);
        b8 found = SERE_FALSE;

        for (u32 j = 0; j < available_layer_count; ++j)
        {
            if (Sere_StrCompare(layer_name, available_layers[j].layerName))
            {
                found = SERE_TRUE;
                SERE_DEBUG("   -> Found");
                break;
            }
        }

        if (!found)
        {
            SERE_FATAL(" Required validation layer is missing: %s", layer_name);
            return SERE_FALSE;
        }
    }
    SERE_DEBUG(" All required validation layers are present.");
#endif

    create_info.enabledLayerCount = required_validation_layer_count;
    create_info.ppEnabledLayerNames = required_validation_layer_names;

    SERE_DEBUG(" Creating Vulkan instance...");
    VkResult result = vkCreateInstance(&create_info, context.allocator, &context.instance);
    if (result != VK_SUCCESS)
    {
        SERE_ERROR(" vkCreateInstance failed. VkResult = %u", result);
        return SERE_FALSE;
    }

#if defined(_DEBUG)
    SERE_DEBUG(" Creating Vulkan debug messenger...");
    u32 log_severity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT |
                       VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT;

    VkDebugUtilsMessengerCreateInfoEXT debug_create_info = {VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT};
    debug_create_info.messageSeverity = log_severity;
    debug_create_info.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                                    VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT |
                                    VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT;
    debug_create_info.pfnUserCallback = Sere_VulkanDebugCallback;
    debug_create_info.pUserData = 0;

    PFN_vkCreateDebugUtilsMessengerEXT func =
        (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(context.instance, "vkCreateDebugUtilsMessengerEXT");
    SERE_ASSERT_MSG(func, "Failed to create debug messenger.");
    SERE_VK_CHECK(func(context.instance, &debug_create_info, context.allocator, &context.debug_messenger));
    SERE_DEBUG(" Vulkan debug messenger created.");
#endif

    SERE_DEBUG(" Creating Vulkan surface...");
    if (!Sere_PlatformCreateVulkanSurface(plat_state, &context))
    {
        SERE_ERROR("Failed to create Vulkan surface.");
    }

    SERE_DEBUG(" Creating Vulkan device...");
    if (!Sere_CreateVulkanDevice(&context))
    {
        SERE_ERROR("Failed to create vulkan device.");
        return SERE_FALSE;
    }

    SERE_DEBUG(" Creating Vulkan swapchain...");
    context.swapchain = *Sere_CreateVulkanSwapchain(
        &context,
        context.framebuffer_width,
        context.framebuffer_height);

    SERE_DEBUG(" Creating Renderpass...");
    context.main_renderpass = *Sere_CreateVulkanRenderPass(
        &context,
        0, 0,
        context.framebuffer_width,
        context.framebuffer_height,
        0.0f, 0.0f, 0.0f,
        1.0f, 1.0f,
        0);

    context.swapchain.framebuffers = Sere_ReserveArray(Sere_VulkanFramebuffer, context.swapchain.image_count);
    Sere_RegenerateFramebuffers(backend, &context.swapchain, &context.main_renderpass);

    SERE_DEBUG(" Creating Command Buffers...");
    Sere_CreateCommandBuffers(backend);

    context.image_available_semaphores = Sere_ReserveArray(VkSemaphore, context.swapchain.max_frames_in_flight);
    context.queue_complete_semaphores = Sere_ReserveArray(VkSemaphore, context.swapchain.image_count);
    context.in_flight_fences = Sere_ReserveArray(Sere_VulkanFence, context.swapchain.max_frames_in_flight);

    for (u8 i = 0; i < context.swapchain.max_frames_in_flight; ++i)
    {
        VkSemaphoreCreateInfo create_info = {VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO};
        SERE_VK_CHECK(vkCreateSemaphore(context.device.logical_device, &create_info, context.allocator, &context.image_available_semaphores[i]));

        context.in_flight_fences[i] = *Sere_CreateVulkanFence(&context, SERE_TRUE);
    }

    for (u32 i = 0; i < context.swapchain.image_count; ++i)
    {
        VkSemaphoreCreateInfo ci = {VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO};
        SERE_VK_CHECK(vkCreateSemaphore(context.device.logical_device, &ci, context.allocator, &context.queue_complete_semaphores[i]));
    }

    context.images_in_flight = Sere_ReserveArray(Sere_VulkanFence, context.swapchain.image_count);
    for (u32 i = 0; i < context.swapchain.image_count; ++i)
    {
        context.images_in_flight[i] = 0;
    }

    context.object_shader = *Sere_CreateVulkanObjectShader(&context);


    SERE_DEBUG(" Vulkan instance created successfully.");
    SERE_DEBUG("============================================================\n");
    return SERE_TRUE;
}

void Sere_VulkanRendererBackendInitialized()
{
    SERE_DEBUG(" Vulkan Renderer Backend fully initialized.");
}

void Sere_ShutdownVulkanRendererBackend(Sere_RendererBackend *backend)
{
    SERE_DEBUG(" Shutting down Vulkan Renderer Backend...");
    vkDeviceWaitIdle(context.device.logical_device);

    Sere_DestroyVulkanObjectShader(&context, &context.object_shader);
    
    for (u8 i = 0; i < context.swapchain.max_frames_in_flight; ++i)
    {
        if (context.image_available_semaphores[i])
        {
            vkDestroySemaphore(
                context.device.logical_device,
                context.image_available_semaphores[i],
                context.allocator);
            context.image_available_semaphores[i] = 0;
        }

        Sere_DestroyVulkanFence(&context, &context.in_flight_fences[i]);
    }
    for (u8 i = 0; i < context.swapchain.image_count; i++)
    {
        if (context.queue_complete_semaphores[i])
        {
            vkDestroySemaphore(
                context.device.logical_device,
                context.queue_complete_semaphores[i],
                context.allocator);
            context.queue_complete_semaphores[i] = 0;
        }
    }
    Sere_DestroyArray(context.image_available_semaphores);
    context.image_available_semaphores = 0;

    Sere_DestroyArray(context.queue_complete_semaphores);
    context.queue_complete_semaphores = 0;

    Sere_DestroyArray(context.in_flight_fences);
    context.in_flight_fences = 0;

    Sere_DestroyArray(context.images_in_flight);
    context.images_in_flight = 0;

    for (u32 i = 0; i < context.swapchain.image_count; ++i)
    {
        if (context.graphics_command_buffers[i].handle)
        {
            Sere_VulkanCommandBufferFree(
                &context,
                context.device.graphics_command_pool,
                &context.graphics_command_buffers[i]);
            context.graphics_command_buffers[i].handle = 0;
        }
    }
    Sere_DestroyArray(context.graphics_command_buffers);
    context.graphics_command_buffers = 0;

    for (u32 i = 0; i < context.swapchain.image_count; ++i)
    {
        Sere_DestroyVulkanFramebuffer(&context, &context.swapchain.framebuffers[i]);
    }

    Sere_DestroyRenderPass(&context, &context.main_renderpass);

    Sere_DestroyVulkanSwapchain(&context, &context.swapchain);

    Sere_DestroyVulkanDevice(&context);
    if (context.surface)
    {
        vkDestroySurfaceKHR(context.instance, context.surface, context.allocator);
        context.surface = VK_NULL_HANDLE;
    }

    if (context.debug_messenger)
    {
        PFN_vkDestroyDebugUtilsMessengerEXT func =
            (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(context.instance, "vkDestroyDebugUtilsMessengerEXT");
        func(context.instance, context.debug_messenger, context.allocator);
    }

    vkDestroyInstance(context.instance, context.allocator);
    SERE_DEBUG(" Vulkan Renderer Backend shutdown complete.");
}

void Sere_VulkanRendererBackendOnResized(Sere_RendererBackend *backend,
                                         u16 width,
                                         u16 height)
{
    SERE_DEBUG(" Window resized: %ux%u", width, height);

    cached_framebuffer_width = width;
    cached_framebuffer_height = height;
    context.framebuffer_size_generation++;
}

b8 Sere_VulkanRendererBackendBeginFrame(Sere_RendererBackend *backend, f32 dt)
{
    Sere_VulkanDevice *device = &context.device;

    if (context.recreating_swapchain)
    {
        VkResult result = vkDeviceWaitIdle(device->logical_device);
        if (!Sere_VulkanResultIsSuccess(result))
        {
            SERE_ERROR("Sere_VulkanRendererBackendBeginFrame vkDeviceWaitIdle (1) failed: '%s'", Sere_VulkanResultString(result, SERE_TRUE));
            return SERE_FALSE;
        }
        return SERE_FALSE;
    }

    if (context.framebuffer_size_generation != context.framebuffer_size_last_generation)
    {
        VkResult result = vkDeviceWaitIdle(device->logical_device);
        if (!Sere_VulkanResultIsSuccess(result))
        {
            SERE_ERROR("Sere_VulkanRendererBackendBeginFrame vkDeviceWaitIdle (2) failed: '%s'", Sere_VulkanResultString(result, SERE_TRUE));
            return SERE_FALSE;
        }

        if (!Sere_RecreateSwapchain(backend))
        {
            return SERE_FALSE;
        }

        return SERE_FALSE;
    }

    if (!Sere_VulkanFenceWait(
            &context,
            &context.in_flight_fences[context.current_frame],
            UINT64_MAX))
    {
        SERE_WARN("In-flight fence wait failure!");
        return SERE_FALSE;
    }

    if (!Sere_VulkanSwapchainAcquireNextImageIndex(
            &context,
            &context.swapchain,
            UINT64_MAX,
            context.image_available_semaphores[context.current_frame],
            0,
            &context.image_index))
    {
        return SERE_FALSE;
    }

    Sere_VulkanCommandBuffer *command_buffer = &context.graphics_command_buffers[context.image_index];
    Sere_VulkanCommandBufferReset(command_buffer);
    Sere_VulkanCommandBufferBegin(command_buffer, SERE_FALSE, SERE_FALSE, SERE_FALSE);

    VkViewport viewport;
    viewport.x = 0.0f;
    viewport.y = (f32)context.framebuffer_height;
    viewport.width = (f32)context.framebuffer_width;
    viewport.height = -(f32)context.framebuffer_height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;

    VkRect2D scissor;
    scissor.offset.x = scissor.offset.y = 0;
    scissor.extent.width = context.framebuffer_width;
    scissor.extent.height = context.framebuffer_height;

    vkCmdSetViewport(command_buffer->handle, 0, 1, &viewport);
    vkCmdSetScissor(command_buffer->handle, 0, 1, &scissor);

    context.main_renderpass.w = context.framebuffer_width;
    context.main_renderpass.h = context.framebuffer_height;

    Sere_VulkanRenderPassBegin(
        command_buffer,
        &context.main_renderpass,
        context.swapchain.framebuffers[context.image_index].handle);

    return SERE_TRUE;
}

b8 Sere_VulkanRendererBackendEndFrame(Sere_RendererBackend *backend, f32 dt)
{
    Sere_VulkanCommandBuffer *command_buffer = &context.graphics_command_buffers[context.image_index];

    Sere_VulkanRenderPassEnd(command_buffer, &context.main_renderpass);

    Sere_VulkanCommandBufferEnd(command_buffer);

    if (context.images_in_flight[context.image_index] != VK_NULL_HANDLE)
    {
        Sere_VulkanFenceWait(&context, context.images_in_flight[context.image_index], UINT64_MAX);
    }

    context.images_in_flight[context.image_index] = &context.in_flight_fences[context.current_frame];

    Sere_VulkanFenceReset(&context, &context.in_flight_fences[context.current_frame]);

    VkSubmitInfo submit_info = {VK_STRUCTURE_TYPE_SUBMIT_INFO};

    submit_info.commandBufferCount = 1;
    submit_info.pCommandBuffers = &command_buffer->handle;

    submit_info.signalSemaphoreCount = 1;
    submit_info.pSignalSemaphores = &context.queue_complete_semaphores[context.image_index];

    submit_info.waitSemaphoreCount = 1;
    submit_info.pWaitSemaphores = &context.image_available_semaphores[context.current_frame];

    VkPipelineStageFlags flags[1] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submit_info.pWaitDstStageMask = flags;

    VkResult result = vkQueueSubmit(
        context.device.graphics_queue,
        1,
        &submit_info,
        context.in_flight_fences[context.current_frame].handle);
    if (result != VK_SUCCESS)
    {
        SERE_ERROR("vkQueueSubmit failed with result: %s", Sere_VulkanResultString(result, SERE_TRUE));
        return SERE_FALSE;
    }

    Sere_VulkanCommandBufferUpdateSubmitted(command_buffer);

    Sere_VulkanSwapchainPresent(
        &context,
        &context.swapchain,
        context.device.graphics_queue,
        context.device.present_queue,
        context.queue_complete_semaphores[context.image_index],
        context.image_index);

    return SERE_TRUE;
}

VKAPI_ATTR VkBool32 VKAPI_CALL Sere_VulkanDebugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
    VkDebugUtilsMessageTypeFlagsEXT message_types,
    const VkDebugUtilsMessengerCallbackDataEXT *callback_data,
    void *user_data)
{
    switch (message_severity)
    {
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
        SERE_ERROR("%s", callback_data->pMessage);
        break;
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
        SERE_WARN("%s", callback_data->pMessage);
        break;
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
        SERE_INFO("%s", callback_data->pMessage);
        break;
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
        SERE_TRACE("%s", callback_data->pMessage);
        break;
    default:
        SERE_DEBUG("%s", callback_data->pMessage);
        break;
    }
    return VK_FALSE;
}

i32 Sere_FindMemoryIndex(u32 type_filter, u32 property_flags)
{
    VkPhysicalDeviceMemoryProperties memory_props;
    vkGetPhysicalDeviceMemoryProperties(context.device.phyiscal_device, &memory_props);

    for (u32 i = 0; i < memory_props.memoryTypeCount; ++i)
    {
        if (type_filter & (1 << i) &&
            (memory_props.memoryTypes[i].propertyFlags & property_flags) == property_flags)
        {
            return i;
        }
    }

    SERE_WARN("Unable to find suitable memory type!");
    return -1;
}

void Sere_CreateCommandBuffers(Sere_RendererBackend *backend)
{
    if (!context.graphics_command_buffers)
    {
        context.graphics_command_buffers = Sere_ReserveArray(Sere_VulkanCommandBuffer, context.swapchain.image_count);
        for (u32 i = 0; i < context.swapchain.image_count; ++i)
        {
            Sere_ZeroMemory(&context.graphics_command_buffers[i], sizeof(Sere_VulkanCommandBuffer));
        }
    }

    for (u32 i = 0; i < context.swapchain.image_count; ++i)
    {
        if (context.graphics_command_buffers[i].handle)
        {
            Sere_VulkanCommandBufferFree(
                &context,
                context.device.graphics_command_pool,
                &context.graphics_command_buffers[i]);
        }
        Sere_ZeroMemory(&context.graphics_command_buffers[i], sizeof(Sere_VulkanCommandBuffer));
        Sere_VulkanCommandBufferAlloc(
            &context,
            context.device.graphics_command_pool,
            SERE_TRUE,
            &context.graphics_command_buffers[i]);
    }
}

void Sere_RegenerateFramebuffers(Sere_RendererBackend *backend, Sere_VulkanSwapchain *swapchain, Sere_VulkanRenderPass *renderpass)
{
    for (u32 i = 0; i < swapchain->image_count; ++i)
    {
        u32 attachment_count = 2;
        VkImageView attachments[] = {
            swapchain->views[i],
            swapchain->depth_attachment.view};

        context.swapchain.framebuffers[i] = *Sere_CreateVulkanFramebuffer(
            &context,
            renderpass,
            context.framebuffer_width,
            context.framebuffer_height,
            attachment_count,
            attachments);
    }
}

b8 Sere_RecreateSwapchain(Sere_RendererBackend *backend)
{
    // Avoid re-entry.
    if (context.recreating_swapchain)
    {
        SERE_DEBUG("Sere_RecreateSwapchain called when already recreating. Booting.");
        return SERE_FALSE;
    }

    // Guard against zero-sized framebuffer.
    if (context.framebuffer_width == 0 || context.framebuffer_height == 0)
    {
        SERE_DEBUG("Sere_RecreateSwapchain called when window extents are < 1 in dimension. Booting.");
        return SERE_FALSE;
    }

    context.recreating_swapchain = SERE_TRUE;

    // Ensure the device is idle before tearing down/recreating.
    vkDeviceWaitIdle(context.device.logical_device);

    // Clear in-flight tracking for all images.
    for (u32 i = 0; i < context.swapchain.image_count; ++i)
    {
        context.images_in_flight[i] = 0; // pointer sentinel; not VK_NULL_HANDLE
    }

    // Free command buffers tied to the old swapchain (they reference old framebuffers).
    for (u32 i = 0; i < context.swapchain.image_count; ++i)
    {
        Sere_VulkanCommandBufferFree(
            &context,
            context.device.graphics_command_pool,
            &context.graphics_command_buffers[i]);
    }

    // Destroy old framebuffers BEFORE touching image views/swapchain.
    for (u32 i = 0; i < context.swapchain.image_count; ++i)
    {
        Sere_DestroyVulkanFramebuffer(&context, &context.swapchain.framebuffers[i]);
    }

    // Refresh swapchain support/depth format.
    Sere_VulkanDeviceQuerySwapchainSupport(
        context.device.phyiscal_device,
        context.surface,
        &context.device.swapchain_support);
    Sere_VulkanDeviceDetectDepthFormat(&context.device);

    // Recreate swapchain with the cached dimensions (destroys/creates image views internally).
    Sere_RecreateVulkanSwapchain(
        &context,
        cached_framebuffer_width,
        cached_framebuffer_height,
        &context.swapchain);

    // Sync framebuffer dimensions with the cached sizes.
    context.framebuffer_width = cached_framebuffer_width;
    context.framebuffer_height = cached_framebuffer_height;
    cached_framebuffer_width = 0;
    cached_framebuffer_height = 0;

    // Update renderpass extents to match the new framebuffer size.
    context.main_renderpass.x = 0;
    context.main_renderpass.y = 0;
    context.main_renderpass.w = context.framebuffer_width;
    context.main_renderpass.h = context.framebuffer_height;

    // Mark resize generation handled.
    context.framebuffer_size_last_generation = context.framebuffer_size_generation;

    // Regenerate framebuffers for the new swapchain image views.
    Sere_RegenerateFramebuffers(backend, &context.swapchain, &context.main_renderpass);

    // Recreate command buffers sized to the new swapchain image count.
    Sere_CreateCommandBuffers(backend);

    context.recreating_swapchain = SERE_FALSE;
    return SERE_TRUE;
}
