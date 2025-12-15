#include "Sere_input.h"
#include "Sere_event.h"
#include "Sere_memory.h"
#include "Sere_logger.h"

// ─────────────────────────────────────────────
// Internal state structures
// ─────────────────────────────────────────────
typedef struct Sere_KeyboardState {
    b8 keys[256];
} Sere_KeyboardState;

typedef struct Sere_MouseState {
    i16 x;
    i16 y;
    u8 buttons[SERE_MAX_MOUSE_BUTTONS];
} Sere_MouseState;

typedef struct Sere_InputState {
    Sere_KeyboardState keyboard_current;
    Sere_KeyboardState keyboard_previous;

    Sere_MouseState mouse_current;
    Sere_MouseState mouse_previous;
} Sere_InputState;

// ─────────────────────────────────────────────
// Static globals
// ─────────────────────────────────────────────
static Sere_InputState state = {};
static b8 is_initialized = SERE_FALSE;

// ─────────────────────────────────────────────
// Lifecycle
// ─────────────────────────────────────────────
void Sere_InitInput() {
    Sere_ZeroMemory(&state, sizeof(Sere_InputState));
    is_initialized = SERE_TRUE;
}

b8 Sere_InputInitialized() {
    return is_initialized;
}

void Sere_ShutdownInput() {
    is_initialized = SERE_FALSE;
}

void Sere_InputUpdate() {
    if (!is_initialized) return;

    Sere_CopyMemory(&state.keyboard_previous, &state.keyboard_current, sizeof(Sere_KeyboardState));
    Sere_CopyMemory(&state.mouse_previous, &state.mouse_current, sizeof(Sere_MouseState));
}

// ─────────────────────────────────────────────
// Keyboard queries
// ─────────────────────────────────────────────
b8 Sere_InputIsKeyDown(Sere_Keys key) {
    return is_initialized && state.keyboard_current.keys[key] == SERE_TRUE;
}

b8 Sere_InputIsKeyUp(Sere_Keys key) {
    return is_initialized && state.keyboard_current.keys[key] == SERE_FALSE;
}

b8 Sere_InputWasKeyDown(Sere_Keys key) {
    return is_initialized && state.keyboard_previous.keys[key] == SERE_TRUE;
}

b8 Sere_InputWasKeyUp(Sere_Keys key) {
    return is_initialized && state.keyboard_previous.keys[key] == SERE_FALSE;
}

// ─────────────────────────────────────────────
// Keyboard processing
// ─────────────────────────────────────────────
void Sere_InputProcessKey(Sere_Keys key, b8 pressed) {
    if (state.keyboard_current.keys[key] != pressed) {
        state.keyboard_current.keys[key] = pressed;

        Sere_EventContext context;
        context.data.u16[0] = key;
        context.key = key;
        Sere_EventFire(pressed ? SERE_EVENT_KEY_PRESSED : SERE_EVENT_KEY_RELEASED, 0, context);
    }
}

// ─────────────────────────────────────────────
// Mouse queries
// ─────────────────────────────────────────────
b8 Sere_InputIsMouseButtonDown(Sere_MouseButtons button) {
    return is_initialized && state.mouse_current.buttons[button] == SERE_TRUE;
}

b8 Sere_InputIsMouseButtonUp(Sere_MouseButtons button) {
    return is_initialized && state.mouse_current.buttons[button] == SERE_FALSE;
}

b8 Sere_InputWasMouseButtonDown(Sere_MouseButtons button) {
    return is_initialized && state.mouse_previous.buttons[button] == SERE_TRUE;
}

b8 Sere_InputWasMouseButtonUp(Sere_MouseButtons button) {
    return is_initialized && state.mouse_previous.buttons[button] == SERE_FALSE;
}

void Sere_InputGetMousePosition(i32 *x, i32 *y) {
    if (!is_initialized) { *x = 0; *y = 0; return; }
    *x = state.mouse_current.x;
    *y = state.mouse_current.y;
}

void Sere_InputGetPreviousMousePosition(i32 *x, i32 *y) {
    if (!is_initialized) { *x = 0; *y = 0; return; }
    *x = state.mouse_previous.x;
    *y = state.mouse_previous.y;
}

// ─────────────────────────────────────────────
// Mouse processing
// ─────────────────────────────────────────────
void Sere_InputProcessMouseButton(Sere_MouseButtons button, b8 pressed) {
    if (state.mouse_current.buttons[button] != pressed) {
        state.mouse_current.buttons[button] = pressed;

        Sere_EventContext context;
        context.data.u16[0] = button;
        Sere_EventFire(pressed ? SERE_EVENT_BUTTON_PRESSED : SERE_EVENT_BUTTON_RELEASED, 0, context);
    }
}

void Sere_InputProcessMouseMove(i16 x, i16 y) {
    if (state.mouse_current.x != x || state.mouse_current.y != y) {
        state.mouse_current.x = x;
        state.mouse_current.y = y;

        Sere_EventContext context;
        context.data.u16[0] = x;
        context.data.u16[1] = y;
        context.mouse.x = x;
        context.mouse.y = y;

        Sere_EventFire(SERE_EVENT_MOUSE_MOTION, 0, context);
    }
}

void Sere_InputProcessMouseWheel(i8 z_delta) {
    Sere_EventContext context;
    context.data.u8[0] = z_delta;
    context.mouse.z_delta = z_delta;
    Sere_EventFire(SERE_EVENT_MOUSE_WHEEL, 0, context);
}
