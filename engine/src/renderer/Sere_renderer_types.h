
#pragma once

#include "Sere_defines.h"
#include "platform/Sere_platform.h"

typedef enum Sere_RendererBackendType
{
    SERE_RENDERER_BACKEND_TYPE_VULKAN,
    SERE_RENDERER_BACKEND_TYPE_OPENGL,
    SERE_RENDERER_BACKEND_TYPE_DIRECTX,
} Sere_RendererBackendType;

typedef struct Sere_RendererBackend
{
    Sere_PlatformState *platform_state;
    u64 frame_number;
    
    b8 (*init)(struct Sere_RendererBackend *backend, const char *app_name, Sere_PlatformState *plat_state);

    void (*shutdown)(struct Sere_RendererBackend *backend);

    void (*resized)(struct Sere_RendererBackend *backend, u16 width, u16 height);

    b8 (*begin_frame)(struct Sere_RendererBackend *backend, f32 dt);
    b8 (*end_frame)(struct Sere_RendererBackend *backend, f32 dt);

} Sere_RendererBackend;

typedef struct Sere_RenderPacket {
    f32 dt;
    
} Sere_RenderPacket;
 