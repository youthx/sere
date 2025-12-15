
#pragma once

#include "Sere_renderer_types.h"

struct Sere_StaticMeshData;
struct Sere_PlatformState;

b8 Sere_InitRenderer(const char *app_name, struct Sere_PlatformState *plat_state);

b8 Sere_RendererInitialized();

void Sere_RendererShutdown();

void Sere_RendererOnResized(u16 width, u16 height);

b8 Sere_RendererDrawFrame(Sere_RenderPacket *packet);
