
#include <Sere.h>

// Entry point: create a window
SERE_RESULT Sere_Entry(Sere_System* system) {
    system->app = Sere_CreateApp("Blank Window", 640, 400);
    return SERE_OK;
}

// Update loop: close when ESC is pressed
SERE_RESULT Sere_Update(Sere_System* system) {
    if (Sere_InputIsKeyDown(SERE_KEY_ESCAPE)) {
        Sere_Quit();
        return SERE_OK;
    }
    return SERE_OK;
}
