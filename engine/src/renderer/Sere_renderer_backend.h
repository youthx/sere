
#pragma once

#include "Sere_renderer_types.h"

struct Sere_PlatformState;

Sere_RendererBackend *Sere_CreateRendererBackend(Sere_RendererBackendType type, struct Sere_PlatformState *plat_state);

void Sere_DestroyRendererBackend(Sere_RendererBackend *backend);
