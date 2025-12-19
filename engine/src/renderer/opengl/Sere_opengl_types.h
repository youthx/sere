
#pragma once

#include "Sere_defines.h"

#include <GL/gl.h>

typedef struct Sere_GLContext {
    void *native_window;
    void *device_context;
    void *gl_context;

    u32 major, minor;
    b8 vsync_enabled;
} Sere_GLContext;



