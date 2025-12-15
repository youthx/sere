
#include "Sere_clipboard.h"

#include <stdint.h>

void Sere_ClipboardProcessUpdate(const char* text, u64 size)
{
    Sere_EventContext context;
    context.data.u64[0] = (u64)(uintptr_t)text;
    context.clipboard.contents = text;
    context.clipboard.length = size;
    Sere_EventFire(SERE_EVENT_CLIPBOARD_UPDATE, 0, context);
}

