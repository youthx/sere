
#include "platform/Sere_platform.h"

#include "core/Sere_logger.h"
#include "core/Sere_input.h"
#include "core/Sere_memory.h"
#include "core/Sere_clipboard.h"

#include "renderer/vulkan/Sere_vulkan_platform.h"

#include "containers/Sere_array.h"

#if KPLATFORM_WINDOWS

#include <windows.h>
#include <windowsx.h>

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_win32.h>

#include "renderer/vulkan/Sere_vulkan_types.h"

typedef struct Sere_InternalState
{
    HINSTANCE h_instance;
    HWND hwnd;
    VkSurfaceKHR surface;
} Sere_InternalState;

static f64 clock_freq;
static LARGE_INTEGER start_time;

LRESULT CALLBACK Sere_Win32ProcessMessage(HWND hwnd, u32 msg, WPARAM w_param, LPARAM l_param);

b8 Sere_PlatformStartup(
    Sere_PlatformState *state,
    const char *title,
    i32 x,
    i32 y,
    i32 width,
    i32 height)
{
    state->internal_state = Sere_Alloc(sizeof(Sere_InternalState), SERE_MEMORY_TAG_PLATFORM);

    Sere_InternalState *internal_state = (Sere_InternalState *)state->internal_state;

    internal_state->h_instance = GetModuleHandleA(0);

    HICON icon = LoadIcon(internal_state->h_instance, IDI_APPLICATION);
    WNDCLASSA wc;
    memset(&wc, 0, sizeof(wc));
    wc.style = CS_DBLCLKS;
    wc.lpfnWndProc = Sere_Win32ProcessMessage;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = internal_state->h_instance;
    wc.hIcon = icon;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = NULL;
    wc.lpszClassName = "Sere_WindowClass";

    if (!RegisterClassA(&wc))
    {
        MessageBoxA(0, "Window registration failed", "Error", MB_ICONEXCLAMATION | MB_OK);
        return SERE_FALSE;
    }

    u32 client_x = x;
    u32 client_y = y;
    u32 client_width = width;
    u32 client_height = height;

    u32 window_x = client_x;
    u32 window_y = client_y;
    u32 window_width = client_width;
    u32 window_height = client_height;

    u32 window_style = WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION;
    u32 window_ex_style = WS_EX_APPWINDOW;

    window_style |= WS_MAXIMIZEBOX;
    window_style |= WS_MINIMIZEBOX;
    window_style |= WS_THICKFRAME;

    RECT border_rect = {0, 0, 0, 0};
    AdjustWindowRectEx(&border_rect, window_style, 0, window_ex_style);

    window_x += border_rect.left;
    window_y += border_rect.top;

    window_width += border_rect.right - border_rect.left;
    window_height += border_rect.bottom - border_rect.top;

    HWND handle = CreateWindowExA(
        window_ex_style, "Sere_WindowClass", title,
        window_style, window_x, window_y, window_width, window_height,
        0, 0, internal_state->h_instance, 0);

    if (handle == 0)
    {
        MessageBoxA(NULL, "Window creation failed!", "Error", MB_ICONEXCLAMATION | MB_OK);

        SERE_FATAL("Window creation failed!");
        return SERE_FALSE;
    }

    internal_state->hwnd = handle;

    AddClipboardFormatListener(internal_state->hwnd);

    b32 should_activate = 1;
    i32 show_window_command_flags = should_activate ? SW_SHOW : SW_SHOWNOACTIVATE;

    ShowWindow(internal_state->hwnd, show_window_command_flags);

    LARGE_INTEGER freq;
    QueryPerformanceFrequency(&freq);
    clock_freq = 1.0 / (f64)freq.QuadPart;
    QueryPerformanceCounter(&start_time);

    return SERE_TRUE;
}

LRESULT CALLBACK Sere_Win32ProcessMessage(HWND hwnd, u32 msg, WPARAM w_param, LPARAM l_param);

void Sere_PlatformShutdown(Sere_PlatformState *state)
{
    Sere_InternalState *internal_state = (Sere_InternalState *)state->internal_state;
    RemoveClipboardFormatListener(internal_state->hwnd);

    if (internal_state->hwnd)
    {
        DestroyWindow(internal_state->hwnd);
        internal_state->hwnd = 0;
    }
}

b8 Sere_PlatformPumpMessages(Sere_PlatformState *state)
{
    MSG msg;
    while (PeekMessageA(&msg, NULL, 0, 0, PM_REMOVE))
    {
        if (msg.message == WM_QUIT)
        {
            return SERE_FALSE; // signal to exit
        }
        TranslateMessage(&msg);
        DispatchMessageA(&msg);
    }
    return SERE_TRUE;
}

void *Sere_PlatformAlloc(u64 size, b8 aligned)
{
    return malloc(size);
}

void Sere_PlatformFree(void *block, b8 aligned)
{
    free(block);
}

void *Sere_PlatformZeroMemory(void *block, u64 size)
{
    return memset(block, 0, size);
}

void *Sere_PlatformCopyMemory(void *dest, const void *source, u64 size)
{
    return memcpy(dest, source, size);
}

void *Sere_PlatformSetMemory(void *dest, i32 value, u64 size)
{
    return memset(dest, value, size);
}

void Sere_PlatformConsoleWrite(const char *message, u8 color)
{
    HANDLE console_handle = GetStdHandle(STD_OUTPUT_HANDLE);
    static u8 levels[6] = {64, 4, 6, 2, 1, 8};
    SetConsoleTextAttribute(console_handle, levels[color]);

    OutputDebugStringA(message);
    u64 length = strlen(message);
    LPDWORD num_written = 0;
    WriteConsoleA(console_handle, message, (DWORD)length, num_written, 0);
}

void Sere_PlatformConsoleWriteError(const char *message, u8 color)
{
    HANDLE console_handle = GetStdHandle(STD_ERROR_HANDLE);
    static u8 levels[6] = {64, 4, 6, 2, 1, 8};
    SetConsoleTextAttribute(console_handle, levels[color]);

    OutputDebugStringA(message);
    u64 length = strlen(message);
    LPDWORD num_written = 0;
    WriteConsoleA(console_handle, message, (DWORD)length, num_written, 0);
}

SERE i32 Sere_GetMonitorWidth()
{
    return (i32)GetSystemMetrics(SM_CXSCREEN);
}

SERE i32 Sere_GetMonitorHeight()
{
    return (i32)GetSystemMetrics(SM_CYSCREEN);
}

f64 Sere_PlatformGetAbsoluteTime()
{
    LARGE_INTEGER now_time;
    QueryPerformanceCounter(&now_time);
    return (f64)now_time.QuadPart * clock_freq;
}

void Sere_PlatformSleep(u64 ms)
{
    Sleep(ms);
}

void Sere_PlatformGetRequiredExtensionNames(const char ***names_array)
{
    Sere_ArrayPush(*names_array, &"VK_KHR_win32_surface");
}

b8 Sere_PlatformCreateVulkanSurface(struct Sere_PlatformState *plat_state, struct Sere_VulkanContext *context)
{
    Sere_InternalState *internal_state = (Sere_InternalState *)plat_state->internal_state;

    VkWin32SurfaceCreateInfoKHR create_info = {VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR};
    create_info.hinstance = internal_state->h_instance;
    create_info.hwnd = internal_state->hwnd;

    VkResult result = vkCreateWin32SurfaceKHR(context->instance, &create_info, context->allocator, &internal_state->surface);
    if (result != VK_SUCCESS)
    {
        SERE_FATAL("Vulkan surface creation failed.");
        return SERE_FALSE;
    }

    context->surface = internal_state->surface;
    return SERE_TRUE;
}

SERE void Sere_PlatformSetTitle(Sere_PlatformState *state, const char *title)
{
    Sere_InternalState *internal_state = (Sere_InternalState *)state->internal_state;
    if (internal_state && internal_state->hwnd)
    {
        SetWindowTextA(internal_state->hwnd, title);
    }
}

LRESULT CALLBACK Sere_Win32ProcessMessage(HWND hwnd, u32 msg, WPARAM w_param, LPARAM l_param)
{
    switch (msg)
    {
    case WM_ERASEBKGND: // Notify the OS that erasing is handled by app to prevent flicker
        return 1;

    case WM_CLOSE:
        DestroyWindow(hwnd);
        return 0;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    case WM_SIZE:
    {
        RECT r;
        GetClientRect(hwnd, &r);
        u32 width = r.right - r.left;
        u32 height = r.bottom - r.top;

        // TODO: Fire an event for window resize
    }
    break;

    case WM_CLIPBOARDUPDATE:
    {
        if (IsClipboardFormatAvailable(CF_TEXT))
        {
            if (OpenClipboard(hwnd))
            {
                HANDLE hData = GetClipboardData(CF_TEXT);
                if (hData)
                {
                    char *pszText = (char *)GlobalLock(hData);
                    if (pszText)
                    {
                        Sere_ClipboardProcessUpdate((const char *)pszText, strlen(pszText));
                        GlobalUnlock(hData);
                    }
                }
                CloseClipboard();
            }
        }
    }
    break;

    case WM_KEYDOWN:
    case WM_SYSKEYDOWN:
    case WM_KEYUP:
    case WM_SYSKEYUP:
    {
        b8 pressed = (msg == WM_KEYDOWN || msg == WM_SYSKEYDOWN);
        Sere_Keys key = (u16)w_param;

        Sere_InputProcessKey(key, pressed);
    }
    break;

    case WM_MOUSEMOVE:
    {
        i32 x_pos = GET_X_LPARAM(l_param);
        i32 y_pos = GET_Y_LPARAM(l_param);

        Sere_InputProcessMouseMove((i16)x_pos, (i16)y_pos);
    }
    break;

    case WM_MOUSEWHEEL:
    {
        i32 z_delta = GET_WHEEL_DELTA_WPARAM(w_param);
        if (z_delta != 0)
        {
            z_delta = (z_delta < 0) ? -1 : 1;
            // TODO: input processing
        }

        Sere_InputProcessMouseWheel((i8)z_delta);
    }
    break;

    case WM_LBUTTONDOWN:
    case WM_MBUTTONDOWN:
    case WM_RBUTTONDOWN:
    case WM_LBUTTONUP:
    case WM_MBUTTONUP:
    case WM_RBUTTONUP:
    {
        b8 pressed = (msg == WM_LBUTTONDOWN || msg == WM_MBUTTONDOWN || msg == WM_RBUTTONDOWN);
        Sere_MouseButtons button = SERE_MAX_MOUSE_BUTTONS;
        switch (msg)
        {
        case WM_LBUTTONDOWN:
        case WM_LBUTTONUP:
            button = SERE_MOUSE_BUTTON_LEFT;
            break;
        case WM_RBUTTONDOWN:
        case WM_RBUTTONUP:
            button = SERE_MOUSE_BUTTON_RIGHT;
            break;
        case WM_MBUTTONDOWN:
        case WM_MBUTTONUP:
            button = SERE_MOUSE_BUTTON_MIDDLE;
            break;
        }

        if (button != SERE_MAX_MOUSE_BUTTONS)
        {
            Sere_InputProcessMouseButton(button, pressed);
        }
    }
    break;
    }

    return DefWindowProcA(hwnd, msg, w_param, l_param);
}

#endif