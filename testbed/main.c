
#include <Sere.h>

// Entry point: create a window
SERE_RESULT Sere_Entry(Sere_System *system)
{
    system->app = Sere_CreateApp("Blank Window", 640, 400);
    return SERE_OK;
}
