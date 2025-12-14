
#pragma once

#include "Sere_defines.h"

typedef struct Sere_App {
    i16 start_pos_x;
    i16 start_pos_y;

    i16 start_width;
    i16 start_height;

    char* name;
} Sere_App;

SERE b8 Sere_CreateApp();

SERE b8 Sere_RunApp();

