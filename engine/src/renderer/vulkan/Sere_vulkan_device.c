
#include "Sere_vulkan_device.h"
#include "core/Sere_logger.h"
#include "core/Sere_string.h"
#include "core/Sere_memory.h"
#include "containers/Sere_array.h"

typedef struct Sere_VulkanPhysicalDeviceRequirements
{
    b8 graphics;
    b8 present;
    b8 compute;
    b8 transfer;

    const char **device_extension_names;
    b8 sampler_anistropy;
    b8 discrete_gpu;
} Sere_VulkanPhysicalDeviceRequirements;

typedef struct Sere_VulkanPhysicalDeviceQueueFamilyInfo
{
    u32 graphics_family_index;
    u32 present_family_index;
    u32 compute_family_index;
    u32 transfer_family_index;
} Sere_VulkanPhysicalDeviceQueueFamilyInfo;

b8 Sere_SelectPhysicalDevice(Sere_VulkanContext *context);
b8 Sere_PhysicalDeviceMeetsRequirements(
    VkPhysicalDevice device,
    VkSurfaceKHR surface,
    const VkPhysicalDeviceProperties *properties,
    const VkPhysicalDeviceFeatures *features,
    const Sere_VulkanPhysicalDeviceRequirements *requirements,
    Sere_VulkanPhysicalDeviceQueueFamilyInfo *out_queue_info,
    Sere_VulkanSwapchainSupportInfo *out_swapchain_support);

b8 Sere_CreateVulkanDevice(Sere_VulkanContext *context)
{
    if (!Sere_SelectPhysicalDevice(context))
    {
        return SERE_FALSE;
    }

    b8 present_shares_graphics_queue = context->device.graphics_queue_index == context->device.present_queue_index;
    b8 transfer_shares_graphics_queue = context->device.graphics_queue_index == context->device.transfer_queue_index;
    u32 index_count = 1;
    if (!present_shares_graphics_queue)
    {
        index_count++;
    }

    if (!transfer_shares_graphics_queue)
    {
        index_count++;
    }

    u32 indices[index_count];
    u8 index = 0;
    indices[index++] = context->device.graphics_queue_index;
    if (!present_shares_graphics_queue)
    {
        indices[index++] = context->device.present_queue_index;
    }
    if (!transfer_shares_graphics_queue)
    {
        indices[index++] = context->device.transfer_queue_index;
    }

    VkDeviceQueueCreateInfo queue_create_infos[index_count];
    f32 queue_priority = 1.0f;

    for (u32 i = 0; i < index_count; ++i)
    {
        queue_create_infos[i] = (VkDeviceQueueCreateInfo){
            .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
            .queueFamilyIndex = indices[i],
            .queueCount = 1,
            .pQueuePriorities = &queue_priority,
            .pNext = 0,
            .flags = 0};
    }

    VkPhysicalDeviceFeatures device_features = {};
    device_features.samplerAnisotropy = VK_TRUE;

    VkDeviceCreateInfo device_create_info = {VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO};
    device_create_info.queueCreateInfoCount = index_count;
    device_create_info.pQueueCreateInfos = queue_create_infos;
    device_create_info.pEnabledFeatures = &device_features;
    device_create_info.enabledExtensionCount = 1;
    const char *extension_names = VK_KHR_SWAPCHAIN_EXTENSION_NAME;
    device_create_info.ppEnabledExtensionNames = &extension_names;

    device_create_info.enabledLayerCount = 0;
    device_create_info.ppEnabledLayerNames = 0;

    SERE_VK_CHECK(vkCreateDevice(
        context->device.phyiscal_device,
        &device_create_info,
        context->allocator,
        &context->device.logical_device));

    vkGetDeviceQueue(
        context->device.logical_device,
        context->device.graphics_queue_index,
        0,
        &context->device.graphics_queue);

    vkGetDeviceQueue(
        context->device.logical_device,
        context->device.present_queue_index,
        0,
        &context->device.present_queue);

    vkGetDeviceQueue(
        context->device.logical_device,
        context->device.transfer_queue_index,
        0,
        &context->device.transfer_queue);

    VkCommandPoolCreateInfo pool_info = {VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO};
    pool_info.queueFamilyIndex = context->device.graphics_queue_index;
    pool_info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

    SERE_VK_CHECK(vkCreateCommandPool(
        context->device.logical_device,
        &pool_info,
        context->allocator,
        &context->device.graphics_command_pool));

    return SERE_TRUE;
}

void Sere_DestroyVulkanDevice(Sere_VulkanContext *context)
{
    if (!context)
        return;

    SERE_INFO("------------------------------------------------------------");
    SERE_INFO("Vulkan teardown: starting device and surface cleanup.");

    /* Clear queue handles (retrieved, not destroyed) */
    context->device.graphics_queue = VK_NULL_HANDLE;
    context->device.present_queue = VK_NULL_HANDLE;
    context->device.transfer_queue = VK_NULL_HANDLE;

    /* 1) Destroy device-side objects (swapchain, etc.) BEFORE the device.
       If you have swapchain, image views, framebuffers, pipelines, cmd pools, etc.,
       destroy them here. */

    /* 2) Destroy logical device (if it exists) */
    if (context->device.logical_device)
    {
        SERE_INFO("Logical device: waiting idle");
        vkDeviceWaitIdle(context->device.logical_device);

        SERE_INFO("Logical device: destroying.");
        vkDestroyDevice(context->device.logical_device, context->allocator);
        context->device.logical_device = VK_NULL_HANDLE;
    }
    else
    {
        SERE_INFO("Logical device: none (already null).");
    }

    /* 3) Destroy surface (it’s an instance child, not device-owned) */
    if (context->surface)
    {
        SERE_INFO("Surface: destroying.");
        vkDestroySurfaceKHR(context->instance, context->surface, context->allocator);
        context->surface = VK_NULL_HANDLE;
    }
    else
    {
        SERE_INFO("Surface: none (already null).");
    }

    /* 4) Free cached swapchain support arrays */
    if (context->device.swapchain_support.formats)
    {
        SERE_INFO("Swapchain support: freeing formats array.");
        Sere_Free(context->device.swapchain_support.formats,
                  sizeof(VkSurfaceFormatKHR) * context->device.swapchain_support.format_count,
                  SERE_MEMORY_TAG_RENDERER);
        context->device.swapchain_support.formats = 0;
        context->device.swapchain_support.format_count = 0;
    }

    if (context->device.swapchain_support.present_modes)
    {
        SERE_INFO("Swapchain support: freeing present modes array.");
        Sere_Free(context->device.swapchain_support.present_modes,
                  sizeof(VkPresentModeKHR) * context->device.swapchain_support.present_mode_count,
                  SERE_MEMORY_TAG_RENDERER);
        context->device.swapchain_support.present_modes = 0;
        context->device.swapchain_support.present_mode_count = 0;
    }

    Sere_ZeroMemory(&context->device.swapchain_support,
                    sizeof(context->device.swapchain_support));

    /* 5) Clear physical device reference (it’s not destroyed explicitly) */
    context->device.phyiscal_device = VK_NULL_HANDLE;
    context->device.graphics_queue_index = -1;
    context->device.present_queue_index = -1;
    context->device.transfer_queue_index = -1;

    SERE_INFO("Vulkan teardown: device and surface cleanup complete.");
    SERE_INFO("------------------------------------------------------------");
}

b8 Sere_SelectPhysicalDevice(Sere_VulkanContext *context)
{
    i32 physical_device_count = 0;
    SERE_VK_CHECK(vkEnumeratePhysicalDevices(context->instance, &physical_device_count, 0));
    if (physical_device_count == 0)
    {
        SERE_FATAL("No devices which support Vulkan were found.");
        return SERE_FALSE;
    }

    VkPhysicalDevice physical_devices[physical_device_count];
    SERE_VK_CHECK(vkEnumeratePhysicalDevices(context->instance, &physical_device_count, physical_devices));
    for (u32 i = 0; i < physical_device_count; i++)
    {
        VkPhysicalDeviceProperties properties;
        vkGetPhysicalDeviceProperties(physical_devices[i], &properties);

        VkPhysicalDeviceFeatures features;
        vkGetPhysicalDeviceFeatures(physical_devices[i], &features);

        VkPhysicalDeviceMemoryProperties memory;
        vkGetPhysicalDeviceMemoryProperties(physical_devices[i], &memory);

        Sere_VulkanPhysicalDeviceRequirements requirements = {};
        requirements.graphics = SERE_TRUE;
        requirements.present = SERE_TRUE;
        requirements.transfer = SERE_TRUE;

        // requirements.compute = SERE_TRUE
        requirements.sampler_anistropy = SERE_TRUE;
        requirements.discrete_gpu = SERE_FALSE;
        requirements.device_extension_names = Sere_CreateArray(const char *);
        Sere_ArrayPush(requirements.device_extension_names, &VK_KHR_SWAPCHAIN_EXTENSION_NAME);

        Sere_VulkanPhysicalDeviceQueueFamilyInfo queue_info = {};
        b8 result = Sere_PhysicalDeviceMeetsRequirements(
            physical_devices[i],
            context->surface,
            &properties,
            &features,
            &requirements,
            &queue_info,
            &context->device.swapchain_support);

        if (result)
        {
            SERE_INFO("Selected device: '%s'.", properties.deviceName);
            switch (properties.deviceType)
            {
            default:
            case VK_PHYSICAL_DEVICE_TYPE_OTHER:
                SERE_INFO("GPU type is Unknown.");
                break;
            case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
                SERE_INFO("GPU type is Integrated.");
                break;
            case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
                SERE_INFO("GPU type is Descrete");
                break;
            case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:
                SERE_INFO("GPU type is Virtual.");
                break;
            case VK_PHYSICAL_DEVICE_TYPE_CPU:
                SERE_INFO("GPU type is CPU.");
                break;
            }

            SERE_INFO(
                "GPU Driver version: %d.%d.%d",
                VK_VERSION_MAJOR(properties.driverVersion),
                VK_VERSION_MINOR(properties.driverVersion),
                VK_VERSION_PATCH(properties.driverVersion));

            SERE_INFO(
                "Vulkan API version: %d.%d.%d",
                VK_VERSION_MAJOR(properties.apiVersion),
                VK_VERSION_MINOR(properties.apiVersion),
                VK_VERSION_PATCH(properties.apiVersion));

            for (u32 j = 0; j < memory.memoryHeapCount; ++j)
            {
                f32 memory_size_gib = (((f32)memory.memoryHeaps[j].size) / 1024.0f / 1024.0f / 1024.0f);
                if (memory.memoryHeaps[j].flags & VK_MEMORY_HEAP_DEVICE_LOCAL_BIT)
                {
                    SERE_INFO("Local GPU memory: %.2f GiB", memory_size_gib);
                }
                else
                {
                    SERE_INFO("Shared System memory: %.2f GiB", memory_size_gib);
                }
            }

            context->device.phyiscal_device = physical_devices[i];
            context->device.graphics_queue_index = queue_info.graphics_family_index;
            context->device.present_queue_index = queue_info.present_family_index;
            context->device.transfer_queue_index = queue_info.transfer_family_index;

            context->device.properties = properties;
            context->device.features = features;
            context->device.memory = memory;
            break;
        }
    }

    if (!context->device.phyiscal_device)
    {
        SERE_ERROR("No physical devices were found which meet the requirements.");
        return SERE_FALSE;
    }

    SERE_INFO("Physical device selected.");
    return SERE_TRUE;
}

void Sere_VulkanDeviceQuerySwapchainSupport(
    VkPhysicalDevice physical_device,
    VkSurfaceKHR surface,
    Sere_VulkanSwapchainSupportInfo *out_support_info)
{
    SERE_VK_CHECK(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(
        physical_device,
        surface,
        &out_support_info->capabilities));

    SERE_VK_CHECK(vkGetPhysicalDeviceSurfaceFormatsKHR(
        physical_device,
        surface,
        &out_support_info->format_count,
        0));

    if (out_support_info->format_count != 0)
    {
        if (!out_support_info->formats)
        {
            out_support_info->formats = Sere_Alloc(sizeof(VkSurfaceFormatKHR) * out_support_info->format_count, SERE_MEMORY_TAG_RENDERER);
        }

        SERE_VK_CHECK(vkGetPhysicalDeviceSurfaceFormatsKHR(
            physical_device,
            surface,
            &out_support_info->format_count,
            out_support_info->formats));
    }

    SERE_VK_CHECK(vkGetPhysicalDeviceSurfacePresentModesKHR(
        physical_device,
        surface,
        &out_support_info->present_mode_count,
        0));
    if (out_support_info->present_mode_count != 0)
    {
        if (!out_support_info->present_modes)
        {
            out_support_info->present_modes = Sere_Alloc(sizeof(VkPresentModeKHR) * out_support_info->present_mode_count, SERE_MEMORY_TAG_RENDERER);
        }

        SERE_VK_CHECK(vkGetPhysicalDeviceSurfacePresentModesKHR(
            physical_device,
            surface,
            &out_support_info->present_mode_count,
            out_support_info->present_modes));
    }
}

b8 Sere_VulkanDeviceDetectDepthFormat(Sere_VulkanDevice *device)
{
    const u64 candidate_count = 3;
    VkFormat candidates[3] = {
        VK_FORMAT_D32_SFLOAT,
        VK_FORMAT_D32_SFLOAT_S8_UINT,
        VK_FORMAT_D24_UNORM_S8_UINT};

    u32 flags = VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT;
    for (u64 i = 0; i < candidate_count; ++i)
    {
        VkFormatProperties props;
        vkGetPhysicalDeviceFormatProperties(device->phyiscal_device, candidates[i], &props);

        if ((props.linearTilingFeatures & flags) == flags)
        {
            device->depth_format = candidates[i];
            return SERE_TRUE;
        }
        else if ((props.optimalTilingFeatures & flags) == flags)
        {
            device->depth_format = candidates[i];
            return SERE_TRUE;
        }
    }
    return SERE_FALSE;
}

b8 Sere_PhysicalDeviceMeetsRequirements(
    VkPhysicalDevice device,
    VkSurfaceKHR surface,
    const VkPhysicalDeviceProperties *properties,
    const VkPhysicalDeviceFeatures *features,
    const Sere_VulkanPhysicalDeviceRequirements *requirements,
    Sere_VulkanPhysicalDeviceQueueFamilyInfo *out_queue_info,
    Sere_VulkanSwapchainSupportInfo *out_swapchain_support)
{
    out_queue_info->graphics_family_index = -1;
    out_queue_info->present_family_index = -1;
    out_queue_info->compute_family_index = -1;
    out_queue_info->transfer_family_index = -1;

    if (requirements->discrete_gpu)
    {
        if (properties->deviceType != VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
        {
            SERE_INFO("Device is not a discrete GPU, and one is required. Skipping.");
            return SERE_FALSE;
        }
    }

    u32 queue_family_count = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, 0);
    VkQueueFamilyProperties queue_families[queue_family_count];
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, queue_families);

    SERE_INFO("Graphics | Present | Compute | Transfer | Name");
    u8 min_transfer_score = 255;
    for (u32 i = 0; i < queue_family_count; ++i)
    {
        u8 current_transfer_score = 0;

        if (queue_families[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
        {
            out_queue_info->graphics_family_index = i;
            ++current_transfer_score;
        }

        if (queue_families[i].queueFlags & VK_QUEUE_COMPUTE_BIT)
        {
            out_queue_info->compute_family_index = i;
            ++current_transfer_score;
        }

        if (queue_families[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
        {
            if (current_transfer_score <= min_transfer_score)
            {
                min_transfer_score = current_transfer_score;
                out_queue_info->transfer_family_index = i;
            }
        }

        VkBool32 supports_present = VK_FALSE;
        SERE_VK_CHECK(vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &supports_present));
        if (supports_present)
        {
            out_queue_info->present_family_index = i;
        }
    }

    SERE_INFO("     %d |        %d |        %d |        %d | %s",
              out_queue_info->graphics_family_index != -1,
              out_queue_info->present_family_index != -1,
              out_queue_info->compute_family_index != -1,
              out_queue_info->transfer_family_index != -1,
              properties->deviceName);

    if (
        (!requirements->graphics || (requirements->graphics && out_queue_info->graphics_family_index != -1)) &&
        (!requirements->present || (requirements->present && out_queue_info->present_family_index != -1)) &&
        (!requirements->compute || (requirements->compute && out_queue_info->compute_family_index != -1)) &&
        (!requirements->transfer || (requirements->transfer && out_queue_info->transfer_family_index != -1)))
    {
        SERE_INFO("Device meets queue requirements.");
        SERE_TRACE("Graphics Family Index: %i", out_queue_info->graphics_family_index);
        SERE_TRACE("Present Family Index: %i", out_queue_info->present_family_index);
        SERE_TRACE("Transfer Family Index: %i", out_queue_info->transfer_family_index);
        SERE_TRACE("Compute Family Index: %i", out_queue_info->compute_family_index);

        Sere_VulkanDeviceQuerySwapchainSupport(
            device,
            surface,
            out_swapchain_support);

        if (out_swapchain_support->format_count < 1 || out_swapchain_support->present_mode_count < 1)
        {
            if (out_swapchain_support->formats)
            {
                Sere_Free(out_swapchain_support->formats, sizeof(VkSurfaceFormatKHR) * out_swapchain_support->format_count, SERE_MEMORY_TAG_RENDERER);
            }

            if (out_swapchain_support->present_modes)
            {
                Sere_Free(out_swapchain_support->present_modes, sizeof(VkPresentModeKHR) * out_swapchain_support->present_mode_count, SERE_MEMORY_TAG_RENDERER);
            }

            SERE_INFO("Required swapchain support not present, skipping device.");
            return SERE_FALSE;
        }

        if (requirements->device_extension_names)
        {
            u32 available_extension_count = 0;
            VkExtensionProperties *available_extensions = 0;
            SERE_VK_CHECK(vkEnumerateDeviceExtensionProperties(
                device,
                0,
                &available_extension_count,
                0));

            if (available_extension_count != 0)
            {
                available_extensions = Sere_Alloc(sizeof(VkExtensionProperties) * available_extension_count, SERE_MEMORY_TAG_RENDERER);
                SERE_VK_CHECK(vkEnumerateDeviceExtensionProperties(
                    device,
                    0,
                    &available_extension_count,
                    available_extensions));

                u32 required_extension_count = Sere_ArrayLength(requirements->device_extension_names);
                for (u32 i = 0; i < required_extension_count; ++i)
                {
                    b8 found = SERE_FALSE;
                    for (u32 j = 0; i < available_extension_count; ++j)
                    {
                        if (Sere_StrCompare(requirements->device_extension_names[i], available_extensions[j].extensionName))
                        {
                            found = SERE_TRUE;
                            break;
                        }
                    }

                    if (!found)
                    {
                        SERE_INFO("Required extension not found: '%s', skipping device.", requirements->device_extension_names[i]);
                        Sere_Free(available_extensions, sizeof(VkExtensionProperties) * available_extension_count, SERE_MEMORY_TAG_RENDERER);
                        return SERE_FALSE;
                    }
                }
            }
            Sere_Free(available_extensions, sizeof(VkExtensionProperties) * available_extension_count, SERE_MEMORY_TAG_RENDERER);
        }

        if (requirements->sampler_anistropy && !features->samplerAnisotropy)
        {
            SERE_INFO("Device does not support samplerAnisotropy, skipping.");
            return SERE_FALSE;
        }

        return SERE_TRUE;
    }
    return SERE_FALSE;
}
