
#pragma once

#include "Sere_defines.h"

typedef struct Sere_App
{
    i16 start_pos_x;
    i16 start_pos_y;

    i16 start_width;
    i16 start_height;

    i16 width;
    i16 height;

    char *title;
} Sere_App;

SERE b8 Sere_Init();

SERE void Sere_Quit();

SERE Sere_App *Sere_CreateApp(const char *title, i32 width, i32 height);

SERE b8 Sere_RunApp();

SERE void Sere_Sleep(u64 ms);

SERE void Sere_SetTitle(const char *title);

void Sere_AppGetFramebufferSize(u32 *width, u32 *height);