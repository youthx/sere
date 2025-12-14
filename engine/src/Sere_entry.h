#pragma once

#include "core/Sere_app.h"
#include "core/Sere_logger.h"
#include "Sere_types.h"

#include <stdio.h>
#include <string.h>

extern SERE_RESULT Sere_Entry(Sere_System *out_system);

void on_resize(Sere_System *system, i16 width, i16 height) {
    if (!system->app) return ;
    system->app->width = width;
    system->app->height = height;
    return;
}

// --- Default fallback functions ---
b8 default_init(Sere_System *system) {
    return SERE_TRUE;
}

b8 default_update(Sere_System *system, f32 delta_time) {
    // Do nothing, keep running
    return SERE_TRUE;
}

b8 default_render(Sere_System *system, f32 delta_time) {
    // Do nothing, no rendering
    return SERE_TRUE;
}

int main(int argc, char** argv) {
    Sere_Init();

    Sere_System system;
    memset(&system, 0, sizeof(system));
    Sere_SetSystem(&system);

    system.argc = argc;
    system.argv = argv;

    // Assign fallbacks before entry
    if (!system.on_resize) system.on_resize = on_resize;
    if (!system.init)      system.init      = default_init;
    if (!system.update)    system.update    = default_update;
    if (!system.render)    system.render    = default_render;

    // Let entry override or extend
    if (!Sere_Entry(&system)) {
        SERE_FATAL("Failed to create system instance.");
        return -1;
    }

    // Ensure fallbacks are still valid after entry
    if (!system.on_resize) system.on_resize = on_resize;
    if (!system.init)      system.init      = default_init;
    if (!system.update)    system.update    = default_update;
    if (!system.render)    system.render    = default_render;

    // Run the app
    Sere_RunApp();
    return 0;
}
