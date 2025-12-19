
#pragma once

#include "Sere_defines.h"

typedef struct Sere_FileHandle {
    void *handle;
    b8 is_valid;
} Sere_FileHandle;

typedef enum Sere_FileMode {
    SERE_FILEMODE_READ = 0x1,
    SERE_FILEMODE_WRITE = 0x2
} Sere_FileMode;

SERE b8 Sere_FileExists(const char *path);

SERE Sere_FileHandle *Sere_OpenFile(const char *path, Sere_FileMode mode, b8 binary);

SERE void Sere_CloseFile(Sere_FileHandle *handle);

SERE b8 Sere_FileReadLine(Sere_FileHandle *handle, char **line_buf);

SERE b8 Sere_FileWriteLine(Sere_FileHandle *handle, const char *text);

SERE b8 Sere_FileRead(Sere_FileHandle *handle, u64 data_size, void *out_data, u64 *out_bytes_read);

SERE b8 Sere_FileReadAllBytes(Sere_FileHandle *handle, u8 **out_bytes, u64 *out_bytes_read);

SERE b8 Sere_FileWrite(Sere_FileHandle *handle, u64 data_size, const char *data, u64 *out_bytes_written);
