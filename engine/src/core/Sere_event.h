
#pragma once

#include "Sere_defines.h"

typedef struct Sere_EventContext {
    
    union {
        i64 i64[2];
        u64 u64[2];
        f64 f64[2];

        i32 i32[4];
        u32 u32[4];
        f32 f32[4];
        
        i16[8];
        u16[8];

        i8 i8[16];
        u8 u8[16];

        char c[16];
    } data;

} Sere_EventContext;

typedef enum Sere_SystemEventCode {
    SERE_EVENT_QUIT = 0x01,
    
    SERE_EVENT_KEY_PRESSED = 0x02,
    SERE_EVENT_KEY_RELEASED = 0x03,
    
    SERE_EVENT_BUTTON_PRESSED = 0x04,
    SERE_EVENT_BUTTON_RELEASED = 0x05,

    SERE_EVENT_MOUSE_MOTION = 0x06,
    SERE_EVENT_MOUSE_WHEEL = 0x07,
    
    SERE_EVENT_RESIZED = 0x08,

    SERE_EVENT_MAX = 0xff
} Sere_SystemEventCode;

typedef b8 (*Sere_PFNOnEvent)(u16 code, void* sender, void* listener_inst, Sere_EventContext data);

SERE b8 Sere_InitEvent();
SERE void Sere_ShutdownEvent();

SERE b8 Sere_EventRegister(u16 code, void* listener, Sere_PFNOnEvent callback);

SERE b8 Sere_EventUnregister(u16 code, void* listener, Sere_PFNOnEvent callback);

SERE b8 Sere_EventFire(u16 code, void* sender, Sere_EventContext context);

