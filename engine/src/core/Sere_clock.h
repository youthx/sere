
#pragma once

#include "Sere_defines.h"

typedef struct Sere_Clock {
    f64 start_time;
    f64 elapsed;

    u32 target_fps;
    f64 dt;
} Sere_Clock;

void Sere_ClockUpdate(Sere_Clock *clock);

void Sere_ClockStart(Sere_Clock *clock);

void Sere_ClockFinish(Sere_Clock *clock);

