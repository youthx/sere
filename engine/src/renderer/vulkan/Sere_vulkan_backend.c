#include "Sere_vulkan_backend.h"
#include "Sere_vulkan_types.h"
#include "Sere_vulkan_platform.h"
#include "Sere_vulkan_device.h"
#include "Sere_vulkan_swapchain.h"

#include "core/Sere_logger.h"
#include "core/Sere_string.h"

#include "containers/Sere_array.h"

static Sere_VulkanContext context;

VKAPI_ATTR VkBool32 VKAPI_CALL Sere_VulkanDebugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
    VkDebugUtilsMessageTypeFlagsEXT message_types,
    const VkDebugUtilsMessengerCallbackDataEXT *callback_data,
    void *user_data);

i32 Sere_FindMemoryIndex(u32 type_filter, u32 property_flags);

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
    context.swapchain = *Sere_CreateVulkanSwapchain(&context, context.framebuffer_width, context.framebuffer_height);
    

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
    // TODO: Handle swapchain recreation
}

b8 Sere_VulkanRendererBackendBeginFrame(Sere_RendererBackend *backend, f32 dt)
{
    // SERE_TRACE(" Begin frame (dt=%.4f)", dt);
    return SERE_TRUE;
}

b8 Sere_VulkanRendererBackendEndFrame(Sere_RendererBackend *backend, f32 dt)
{
    // SERE_TRACE(" End frame (dt=%.4f)", dt);
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

