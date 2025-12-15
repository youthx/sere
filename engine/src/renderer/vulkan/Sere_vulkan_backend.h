
#pragma once

#include "renderer/Sere_renderer_backend.h"

b8 Sere_InitVulkanRendererBackend(Sere_RendererBackend *backend, const char *app_name, struct Sere_PlatformState *plat_state);
void Sere_ShutdownVulkanRendererBackend(Sere_RendererBackend *backend);


void Sere_VulkanRendererBackendInitialized();
void Sere_VulkanRendererBackendOnResized(Sere_RendererBackend *backend, u16 width, u16 height);

b8 Sere_VulkanRendererBackendBeginFrame(Sere_RendererBackend *backend, f32 dt);
b8 Sere_VulkanRendererBackendEndFrame(Sere_RendererBackend *backend, f32 dt);

