
#include "Sere_version.h"
#include "Sere_app.h"
#include "Sere_logger.h"

#include "platform/Sere_platform.h"

#include "Sere_types.h"
#include "Sere_system.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct Sere_AppState
{
    b8 is_running;
    b8 is_suspended;

    Sere_PlatformState platform;
    Sere_System *system;

    i16 width;
    i16 height;
    f64 last_time;
} Sere_AppState;

static b8 initialized = SERE_FALSE;
static b8 sere_initialized = SERE_FALSE;

static Sere_AppState app_state;

b8 Sere_SetSystem(Sere_System *system)
{
    app_state.system = system;
    app_state.system->state = Sere_PlatformAlloc(sizeof(Sere_SystemState), SERE_FALSE);
    
    return app_state.system != NULL;
}

b8 Sere_Init()
{

    Sere_InitLogging();

    #ifndef SERE_SILENT
    printf("sere %s\nHello from the Sere commmunity.", SERE_VERSION);
    #endif 

    sere_initialized = SERE_TRUE;
}

Sere_App *Sere_CreateApp(const char *title, i32 width, i32 height)
{
    if (initialized)
    {
        SERE_ERROR("Sere_Create called more than once.");
        return NULL;
    }

    if (!sere_initialized)
    {
        SERE_ERROR("Sere subsystems not initialized.");
        return NULL;
    }

    i32 x = (Sere_GetScreenWidth() - width) / 2;
    i32 y = (Sere_GetScreenHeight() - height) / 2;

    Sere_App *app = (Sere_App *)Sere_PlatformAlloc(sizeof(Sere_App), SERE_FALSE);
    app->title = malloc(strlen(title));
    strcpy(app->title, title);

    app->start_pos_x = x;
    app->start_pos_y = y;
    app->start_width = width;
    app->start_height = height;

    app->width = width;
    app->height = height;

    app_state.is_running = SERE_TRUE;
    app_state.is_suspended = SERE_FALSE;

    if (!Sere_PlatformStartup(&app_state.platform, title, x, y, width, height))
    {
        return NULL;
    }

    
    if (!app_state.system->init(app_state.system))
    {
        SERE_FATAL("App failed to initialize.");
        return NULL;
    }
  
    app_state.system->on_resize(app_state.system, app_state.width, app_state.height);

    initialized = SERE_TRUE;
    return app;
}

b8 Sere_RunApp()
{
    while (app_state.is_running)
    {
        if (!Sere_PlatformPumpMessages(&app_state.platform))
        {
            app_state.is_running = SERE_FALSE;
        }

        if (!app_state.is_suspended)
        {
            if (!app_state.system->update(app_state.system, (f32)0)) {
                SERE_FATAL("App update failed, shutting down.");
                app_state.is_running = SERE_FALSE;
                break;
            }
            
            if (!app_state.system->render(app_state.system, (f32)0)) {
                SERE_FATAL("App render failed, shutting down.");
                app_state.is_running = SERE_FALSE;
                break;
            }
            
        }
    }

    app_state.is_running = SERE_FALSE;
    Sere_PlatformShutdown(&app_state.platform);

    return SERE_TRUE;
}
