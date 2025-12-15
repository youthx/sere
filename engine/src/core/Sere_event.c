
#include "core/Sere_event.h"
#include "core/Sere_memory.h"

#include "containers/Sere_array.h"

typedef struct Sere_RegisteredEvent
{
    void *listener;
    Sere_PFNOnEvent callback;
} Sere_RegisteredEvent;

typedef struct Sere_EventCodeEntry
{
    Sere_RegisteredEvent *events;
} Sere_EventCodeEntry;

#define MAX_MESSAGE_CODES 16384

typedef struct Sere_EventSystemState
{
    Sere_EventCodeEntry registered[MAX_MESSAGE_CODES];
} Sere_EventSystemState;

static b8 is_initialized = SERE_FALSE;
static Sere_EventSystemState state;

b8 Sere_InitEvent()
{
    if (is_initialized == SERE_TRUE)
    {
        return SERE_FALSE;
    }

    is_initialized = SERE_FALSE;
    Sere_ZeroMemory(&state, sizeof(state));

    is_initialized = SERE_TRUE;
    return SERE_TRUE;
}

SERE b8 Sere_EventInitialized()
{
    return is_initialized;
}

void Sere_ShutdownEvent()
{
    for (u16 i = 0; i < MAX_MESSAGE_CODES; ++i)
    {
        if (state.registered[i].events != 0)
        {
            Sere_DestroyArray(state.registered[i].events);
            state.registered[i].events = 0;
        }
    }
}

b8 Sere_EventRegister(u16 code, void *listener, Sere_PFNOnEvent callback)
{
    if (is_initialized == SERE_FALSE)
    {
        return SERE_FALSE;
    }

    if (state.registered[code].events == 0)
    {
        state.registered[code].events = Sere_CreateArray(Sere_RegisteredEvent);
    }

    u64 registered_count = Sere_ArrayLength(state.registered[code].events);
    for (u64 i = 0; i < registered_count; ++i)
    {
        if (state.registered[code].events[i].listener == listener)
        {
            return SERE_FALSE;
        }
    }

    Sere_RegisteredEvent event;
    event.listener = listener;
    event.callback = callback;
    Sere_ArrayPush(state.registered[code].events, event);
    return SERE_TRUE;
}

b8 Sere_EventUnregister(u16 code, void *listener, Sere_PFNOnEvent callback)
{
    if (is_initialized == SERE_FALSE)
    {
        return SERE_FALSE;
    }

    if (state.registered[code].events == 0)
    {
        return SERE_FALSE;
    }

    u64 registered_count = Sere_ArrayLength(state.registered[code].events);
    for (u64 i = 0; i < registered_count; ++i)
    {
        Sere_RegisteredEvent e = state.registered[code].events[i];
        if (e.listener == listener && e.callback == callback)
        {
            Sere_RegisteredEvent popped_event;
            Sere_ArrayPopAt(state.registered[code].events, i, &popped_event);
            return SERE_TRUE;
        }
    }

    return SERE_FALSE;
}

b8 Sere_EventFire(u16 code, void *sender, Sere_EventContext context)
{
    context.type = code;
    if (is_initialized == SERE_FALSE)
    {
        return SERE_FALSE;
    }

    if (state.registered[code].events == 0)
    {
        return SERE_FALSE;
    }

    u64 registered_count = Sere_ArrayLength(state.registered[code].events);
    for (u64 i = 0; i < registered_count; i++)
    {
        Sere_RegisteredEvent e = state.registered[code].events[i];
        if (e.callback(code, sender, e.listener, context))
        {
            return SERE_TRUE;
        }
    }

    return SERE_FALSE;
}
