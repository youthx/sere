
#include "Sere_vulkan_backend.h"
#include "Sere_vulkan_types.inl"

#include "core/Sere_logger.h"

static Sere_VulkanContext context;

b8 Sere_InitVulkanRendererBackend(Sere_RendererBackend *backend, const char *app_name, struct Sere_PlatformState *plat_state)
{
    context.allocator = 0;

    VkApplicationInfo app_info = {VK_STRUCTURE_TYPE_APPLICATION_INFO};
    app_info.apiVersion = VK_API_VERSION_1_2;
    app_info.pApplicationName = app_name;
    app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    app_info.pEngineName = "Sere Engine";
    app_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    
    VkInstanceCreateInfo create_info = {VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO};
    create_info.pApplicationInfo = &app_info;
    create_info.enabledExtensionCount = 0;
    create_info.ppEnabledExtensionNames = 0;
    create_info.enabledLayerCount = 0;
    create_info.ppEnabledLayerNames = 0;

    VkResult result = vkCreateInstance(&create_info, context.allocator, &context.instance);
    if (result != VK_SUCCESS) {
        SERE_ERROR("vkCreateInstance failed with result: %u", result);
        return SERE_FALSE;
    }
    SERE_INFO("Vulkan renderer initialized successfully.");
    return SERE_TRUE;
}

void Sere_VulkanRendererBackendInitialized()
{

}

void Sere_ShutdownVulkanRendererBackend(Sere_RendererBackend *backend)
{

}

void Sere_VulkanRendererBackendOnResized(Sere_RendererBackend *backend, u16 width, u16 height)
{

}

b8 Sere_VulkanRendererBackendBeginFrame(Sere_RendererBackend *backend, f32 dt)
{
    return SERE_TRUE;
}

b8 Sere_VulkanRendererBackendEndFrame(Sere_RendererBackend *backend, f32 dt)
{
    return SERE_TRUE;
}

