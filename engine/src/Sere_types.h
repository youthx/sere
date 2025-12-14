#pragma once

#include "core/Sere_app.h"

typedef struct Sere_System
{

    Sere_App *app;

    int argc;
    char **argv;

    b8 (*init)(struct Sere_System *system);

    b8 (*update)(struct Sere_System *system, f32 dt);

    b8 (*render)(struct Sere_System *system, f32 dt);

    void (*on_resize)(struct Sere_System *system, i16 width, i16 height);

    void *state;
} Sere_System;

SERE b8 Sere_SetSystem(Sere_System *system);
