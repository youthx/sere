
#include "core/Sere_event.h"
#include "core/Sere_memory.h"

typedef struct Sere_RegisteredEvent {
    void* listener;
    Sere_PFNOnEvent callback;
} Sere_RegisteredEvent;

typedef struct Sere_EventCodeEntry {
    Sere_RegisteredEvent* events;
} Sere_EventCodeEntry;

#define MAX_MESSAGE_CODES 16384

typedef struct Sere_EventSystemState {
    Sere_EventCodeEntry registered[MAX_MESSAGE_CODES];
} Sere_EventSystemState;

static b8 is_initialized = SERE_FALSE 
static Sere_EventSystemState state;



