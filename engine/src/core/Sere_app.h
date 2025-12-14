
#pragma once

#include "Sere_defines.h"

typedef struct Sere_App {
    i16 start_pos_x;
    i16 start_pos_y;

    i16 start_width;
    i16 start_height;

    i16 width;
    i16 height;

    char* title;
} Sere_App;

SERE b8 Sere_Init();

SERE Sere_App* Sere_CreateApp(const char* title, i32 width, i32 height);

SERE b8 Sere_RunApp();

