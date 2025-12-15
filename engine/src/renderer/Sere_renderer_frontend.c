
#include "Sere_renderer_frontend.h"
#include "Sere_renderer_backend.h"

#include "core/Sere_logger.h"
#include "core/Sere_memory.h"

static Sere_RendererBackend *backend;
static b8 is_initialized = SERE_FALSE;

b8 Sere_InitRenderer(const char *app_name, struct Sere_PlatformState *plat_state)
{
    backend = Sere_CreateRendererBackend(SERE_RENDERER_BACKEND_TYPE_VULKAN, plat_state);
    if (!backend->init(backend, app_name, plat_state)) {
        SERE_FATAL("Renderer backend failed to initialize.");
        return SERE_FALSE;
    }

    is_initialized = SERE_TRUE;
    return SERE_TRUE;
}

b8 Sere_RendererInitialized()
{
    return is_initialized;
}

void Sere_RendererShutdown()
{
    backend->shutdown(backend);
    Sere_Free(backend, sizeof(Sere_RendererBackend), SERE_MEMORY_TAG_RENDERER);
    
    is_initialized = SERE_FALSE;
}

void Sere_RendererOnResized(u16 width, u16 height);

b8 Sere_RendererBeginFrame(f32 dt) 
{
    return backend->begin_frame(backend, dt);
}

b8 Sere_RendererEndFrame(f32 dt) 
{
    b8 result = backend->end_frame(backend, dt);
    backend->frame_number++;
    return result;
}


b8 Sere_RendererDrawFrame(Sere_RenderPacket *packet)
{
    
    if (Sere_RendererBeginFrame(packet->dt)) {
        
        b8 result = Sere_RendererEndFrame(packet->dt);

        if (!result) {
            SERE_ERROR("Sere_RendererEndFrame failed.");
            return SERE_FALSE;
        }
        
    }

    return SERE_TRUE;
}