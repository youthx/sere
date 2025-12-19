#pragma once

#include "core/Sere_app.h"
#include "core/Sere_logger.h"
#include "Sere_types.h"

#include <stdio.h>
#include <string.h>

// ─────────────────────────────────────────────
// Weak defaults: can be overridden by user/game
// ─────────────────────────────────────────────
SERE_RESULT Sere_Entry(Sere_System *system) SERE_WEAK;
SERE_RESULT Sere_Update(Sere_System *system) SERE_WEAK;
SERE_RESULT Sere_Render(Sere_System *system) SERE_WEAK;

// ─────────────────────────────────────────────
// Default callbacks
// ─────────────────────────────────────────────
void on_resize(Sere_System *system, i16 width, i16 height)
{
    if (!system->app)
        return;
    system->app->width = width;
    system->app->height = height;
}

SERE_RESULT Sere_Default_Entry(Sere_System *system)
{
    return SERE_OK;
}

SERE_RESULT Sere_Default_Update(Sere_System *system)
{
    return SERE_OK;
}

SERE_RESULT Sere_Default_Render(Sere_System *system)
{
    return SERE_OK;
}

SERE_ALIAS(Sere_Entry, Sere_Default_Entry)
SERE_ALIAS(Sere_Update, Sere_Default_Update)
SERE_ALIAS(Sere_Render, Sere_Default_Render)

// Wrappers to adapt Sere_Update/Sere_Render (SERE_RESULT) to b8 signatures
static b8 wrap_update(Sere_System *sys, f32 dt)
{
    sys->dt = dt;
    if (!Sere_Update)
        return Sere_Default_Update(sys);
    return Sere_Update(sys);
}

static b8 wrap_render(Sere_System *sys, f32 dt)
{
    sys->dt = dt;
    if (!Sere_Render)
        return Sere_Default_Render(sys);
    return Sere_Render(sys);
}

// ─────────────────────────────────────────────
// Entry point
// ─────────────────────────────────────────────
int main(int argc, char **argv)
{
    Sere_Init();

    Sere_System system;
    memset(&system, 0, sizeof(system));
    Sere_SetSystem(&system);

    system.argc = argc;
    system.argv = argv;

    // Always assign safe defaults first
    system._on_resize = on_resize;

    // Override with externs (weak defaults will be replaced if user provides)
    system._update = wrap_update;
    system._render = wrap_render;

    // Entry always exists (weak default if not overridden)
    if (!Sere_Entry(&system))
    {
        SERE_FATAL("Failed to create system instance.");
        return -1;
    }

    // Run the app
    Sere_RunApp();
    return 0;
}
