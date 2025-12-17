
#pragma once

#include "Sere_defines.h"

struct Sere_PlatformState;
struct Sere_VulkanContext;

b8 Sere_PlatformCreateVulkanSurface(
    struct Sere_PlatformState *plat_state,
    struct Sere_VulkanContext *context);

void Sere_PlatformGetRequiredExtensionNames(const char ***names_array);
