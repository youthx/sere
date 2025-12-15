#pragma once

#include "core/Sere_app.h"
#include "core/Sere_clock.h"

typedef struct Sere_System
{

    Sere_App *app;
    Sere_Clock clock;
    
    int argc;
    char **argv;

    b8 (*_init)(struct Sere_System *system);

    b8 (*_update)(struct Sere_System *system, f32 dt);

    b8 (*_render)(struct Sere_System *system, f32 dt);

    void (*_on_resize)(struct Sere_System *system, i16 width, i16 height);

    void *state;

    f64 dt;
} Sere_System;

SERE b8 Sere_SetSystem(Sere_System *system);
