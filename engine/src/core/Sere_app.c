// Core engine headers
#include "Sere_version.h"
#include "core/Sere_app.h"
#include "core/Sere_logger.h"
#include "core/Sere_memory.h"
#include "core/Sere_event.h"
#include "core/Sere_input.h"
#include "core/Sere_clock.h"

#include "Sere_types.h"
#include "Sere_system.h"

// Renderer
#include "renderer/Sere_renderer_frontend.h"

// Platform layer
#include "platform/Sere_platform.h"


// Standard library
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// -------------------------------------------------------------------------------------------------
// Application state
// -------------------------------------------------------------------------------------------------

typedef struct Sere_AppState {
    b8 is_running;
    b8 is_suspended;

    Sere_PlatformState platform;
    Sere_System* system;

    i16 width;
    i16 height;

    Sere_Clock clock;
    f64 last_time;
} Sere_AppState;

// -------------------------------------------------------------------------------------------------
// Static globals
// -------------------------------------------------------------------------------------------------

static b8 initialized = SERE_FALSE;
static b8 sere_initialized = SERE_FALSE;
static Sere_AppState app_state;

// -------------------------------------------------------------------------------------------------
// System setup and teardown
// -------------------------------------------------------------------------------------------------

b8 Sere_SetSystem(Sere_System* system) {
    app_state.system = system;
    app_state.system->state = Sere_PlatformAlloc(sizeof(Sere_SystemState), SERE_FALSE);
    return app_state.system != NULL;
}

b8 Sere_Init() {
    if (!Sere_MemoryInitialized()) Sere_InitMemory();
    if (!Sere_LoggingInitialized()) Sere_InitLogging();
    if (!Sere_EventInitialized()) Sere_InitEvent();
    if (!Sere_InputInitialized()) Sere_InitInput();

#ifndef SERE_SILENT
    printf("sere %s\nHello from the Sere community.\n", SERE_VERSION);
#endif

    sere_initialized = SERE_TRUE;
    return SERE_TRUE;
}

void Sere_Quit() {
    app_state.is_running = SERE_FALSE;

    Sere_ShutdownInput();
    Sere_ShutdownEvent();
    Sere_RendererShutdown();
    Sere_ShutdownLogging();
    Sere_ShutdownMemory();
    Sere_PlatformShutdown(&app_state.platform);
}

// -------------------------------------------------------------------------------------------------
// Convenience wrappers
// -------------------------------------------------------------------------------------------------

void Sere_Sleep(u64 ms) {
    Sere_PlatformSleep(ms);
}

void Sere_SetTitle(const char* title) {
    Sere_PlatformSetTitle(&app_state.platform, title);
}

// -------------------------------------------------------------------------------------------------
// Application creation
// -------------------------------------------------------------------------------------------------

Sere_App* Sere_CreateApp(const char* title, i32 width, i32 height) {
    if (initialized) {
        SERE_ERROR("Sere_Create called more than once.");
        return NULL;
    }
    if (!sere_initialized) {
        SERE_ERROR("Sere subsystems not initialized.");
        return NULL;
    }

    i32 x = (Sere_GetMonitorWidth() - width) / 2;
    i32 y = (Sere_GetMonitorHeight() - height) / 2;

    Sere_App* app = (Sere_App*)Sere_PlatformAlloc(sizeof(Sere_App), SERE_FALSE);
    if (!app) {
        SERE_FATAL("Failed to allocate Sere_App.");
        return NULL;
    }

    app->title = (char*)Sere_Alloc(strlen(title) + 1, SERE_MEMORY_TAG_STRING);
    strcpy(app->title, title);

    app->start_pos_x = x;
    app->start_pos_y = y;
    app->start_width = width;
    app->start_height = height;
    app->width = width;
    app->height = height;

    app_state.is_running = SERE_TRUE;
    app_state.is_suspended = SERE_FALSE;
    app_state.width = (i16)width;
    app_state.height = (i16)height;

    if (!Sere_PlatformStartup(&app_state.platform, title, x, y, width, height)) {
        app_state.is_running = SERE_FALSE;
        return NULL;
    }

    // Renderer startup 
    if (!Sere_InitRenderer(title, &app_state.platform)) {
        SERE_FATAL("Failed to initialize renderer. Aborting application.");
        app_state.is_running = SERE_FALSE;
        return NULL;
    }
    
    if (!app_state.system->state) {
        app_state.system->state = Sere_Alloc(sizeof(Sere_SystemState), SERE_MEMORY_TAG_SYSTEM);
    }
    app_state.system->app = app;

    if (app_state.system->_init) {
        if (!app_state.system->_init(app_state.system)) {
            SERE_FATAL("App failed to initialize.");
            app_state.is_running = SERE_FALSE;
            return NULL;
        }
    }

    if (app_state.system->_on_resize) {
        app_state.system->_on_resize(app_state.system, app_state.width, app_state.height);
    }

    initialized = SERE_TRUE;
    return app;
}

// -------------------------------------------------------------------------------------------------
// Application run loop
// -------------------------------------------------------------------------------------------------

b8 Sere_RunApp() {
    Sere_ClockStart(&app_state.clock);
    Sere_ClockUpdate(&app_state.clock);

    // R95: Establish initial timing baseline before entering the main loop.
    app_state.last_time = app_state.clock.elapsed;
    f64 running_time = 0;
    u8 frame_count = 0;
    f64 target_frame_seconds = 1.0 / 60.0;

    SERE_INFO(Sere_GetMemoryUsageString());

    if (!initialized || !app_state.is_running) {
        return SERE_FALSE;
    }

    while (app_state.is_running) {
        if (!Sere_PlatformPumpMessages(&app_state.platform)) {
            app_state.is_running = SERE_FALSE;
            break;
        }

        if (!app_state.is_suspended) {
            Sere_ClockUpdate(&app_state.clock);
            f64 current_time = app_state.clock.elapsed;
            f64 delta = current_time - app_state.last_time;
            app_state.clock.dt = delta;

            f64 frame_start_time = Sere_PlatformGetAbsoluteTime();

            if (app_state.system->_update &&
                !app_state.system->_update(app_state.system, (f32)delta)) {
                SERE_FATAL("App update failed, shutting down.");
                app_state.is_running = SERE_FALSE;
                break;
            }

            if (app_state.system->_render &&
                !app_state.system->_render(app_state.system, (f32)delta)) {
                SERE_FATAL("App render failed, shutting down.");
                app_state.is_running = SERE_FALSE;
                break;
            }

            // Renderer frame submission
            Sere_RenderPacket packet;
            packet.dt = delta;
            Sere_RendererDrawFrame(&packet);

            f64 frame_end_time = Sere_PlatformGetAbsoluteTime();
            f64 frame_elapsed_time = frame_end_time - frame_start_time;
            running_time += frame_elapsed_time;
            f64 remaining_seconds = target_frame_seconds - frame_elapsed_time;
            
            if (remaining_seconds > 0.0) {
                u64 remaining_ms = (u64)(remaining_seconds * 1000.0);
                b8 limit_frames = SERE_FALSE;
                if (remaining_ms > 1 && limit_frames) {
                    Sere_PlatformSleep(remaining_ms - 1);
                }

                frame_count++;
            }

            Sere_InputUpdate();
            app_state.last_time = current_time;
        }
    }

    Sere_Quit();
    return SERE_TRUE;
}
