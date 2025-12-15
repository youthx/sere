
#include "Sere_clock.h"

#include "platform/Sere_platform.h"

void Sere_ClockUpdate(Sere_Clock *clock)
{
    if (clock->start_time != 0)
    {
        clock->elapsed = Sere_PlatformGetAbsoluteTime() - clock->start_time;
    }
}


void Sere_ClockStart(Sere_Clock *clock)
{
    clock->start_time = Sere_PlatformGetAbsoluteTime();
    clock->elapsed = 0;
    clock->target_fps = 60;
}

void Sere_ClockFinish(Sere_Clock *clock)
{
    clock->start_time = 0;
}




