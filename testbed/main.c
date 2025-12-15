# sere
A lightweight, modular **game engine** written in C.

---

## Overview
`sere` is an experimental game engine project designed around simplicity, modularity, and clarity.  
It provides a foundation for building cross‑platform applications with a clean separation between **core systems**, **platform abstraction**, and **rendering backends**.

The engine is still in early development, but it already supports:
- Core subsystems for memory, logging, events, and input.
- A platform layer for window creation and message handling.
- An application lifecycle manager (`Sere_App`) with run loop support.
- A Vulkan backend with instance creation, extension enumeration, validation layers, and debug messenger integration.

---

## Current Capabilities
- **Window creation**: Create a resizable application window with `Sere_CreateApp`.
- **Application lifecycle**: Initialize subsystems, run the main loop, and shut down cleanly.
- **Input handling**: Detect key presses (e.g., closing the app when `ESC` is pressed).
- **Logging system**: Configurable log levels (`FATAL`, `ERROR`, `WARN`, `INFO`, `DEBUG`, `TRACE`).
- **Vulkan backend**: Instance creation, extension and validation layer checks, debug messenger support.

---

## Quick Start Example

Here’s a minimal program using `sere`:

```c
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
