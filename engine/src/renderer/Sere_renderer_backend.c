
#include "Sere_renderer_backend.h"
#include "core/Sere_memory.h"

#include "vulkan/Sere_vulkan_backend.h"
#include "opengl/Sere_opengl_backend.h"

#include <stdlib.h>

Sere_RendererBackend *Sere_CreateRendererBackend(Sere_RendererBackendType type, struct Sere_PlatformState *plat_state)
{
    Sere_RendererBackend *backend = (Sere_RendererBackend*)Sere_Alloc(sizeof(Sere_RendererBackend), SERE_MEMORY_TAG_RENDERER);
    backend->platform_state = plat_state;
    backend->frame_number = 0;

    if (type == SERE_RENDERER_BACKEND_TYPE_VULKAN) {
        backend->init = Sere_InitVulkanRendererBackend;
        backend->shutdown = Sere_ShutdownVulkanRendererBackend;
        backend->resized = Sere_VulkanRendererBackendOnResized;
        backend->begin_frame = Sere_VulkanRendererBackendBeginFrame;
        backend->end_frame = Sere_VulkanRendererBackendEndFrame;
        
        return backend;
    }
    else if (type == SERE_RENDERER_BACKEND_TYPE_OPENGL) {
        backend->init = Sere_InitOpenGLRendererBackend;
        backend->shutdown = Sere_ShutdownOpenGLRendererBackend;
        backend->resized = SereOpenGLRendererBackendOnResized;
        backend->begin_frame = SereOpenGLRendererBackendBeginFrame;
        backend->end_frame = Sere_OpenGLRendererBackendEndFrame;
    }
    
    return NULL;
}

void Sere_DestroyRendererBackend(Sere_RendererBackend *backend)
{
    backend->init = 0;
    backend->shutdown = 0;
    backend->begin_frame = 0;
    backend->end_frame = 0;
    backend->resized = 0;
}