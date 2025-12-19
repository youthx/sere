
#include "Sere_filesystem.h"

#include "core/Sere_logger.h"
#include "core/Sere_memory.h"

#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

b8 Sere_FileExists(const char *path)
{
    struct stat buffer;
    return stat(path, &buffer) == 0;
}

Sere_FileHandle *Sere_OpenFile(const char *path, Sere_FileMode mode, b8 binary)
{
    Sere_FileHandle *out_handle = (Sere_FileHandle *)Sere_Alloc(sizeof(Sere_FileHandle), SERE_MEMORY_TAG_PLATFORM);
    out_handle->is_valid = SERE_FALSE;
    out_handle->handle = 0;
    const char *mode_str;

    if ((mode & SERE_FILEMODE_READ) != 0 && (mode & SERE_FILEMODE_WRITE) != 0)
    {
        mode_str = binary ? "w+b" : "w+";
    }
    else if ((mode & SERE_FILEMODE_READ) != 0 && (mode & SERE_FILEMODE_WRITE) == 0)
    {
        mode_str = binary ? "rb" : "r";
    }
    else if ((mode & SERE_FILEMODE_READ) == 0 && (mode & SERE_FILEMODE_WRITE) != 0)
    {
        mode_str = binary ? "wb" : "w";
    }
    else
    {
        SERE_ERROR("Invalid mode passed while trying to open file: '%s'", path);
        return NULL;
    }

    FILE *file = fopen(path, mode_str);
    if (!file)
    {
        SERE_ERROR("Error opening file: '%s'", path);
        return NULL;
    }

    out_handle->handle = file;
    out_handle->is_valid = SERE_TRUE;

    return out_handle;
}

void Sere_CloseFile(Sere_FileHandle *handle)
{
    if (handle->handle)
    {
        fclose((FILE *)handle->handle);
        handle->handle = 0;
        handle->is_valid = SERE_FALSE;
    }
}

b8 Sere_FileReadLine(Sere_FileHandle *handle, char **line_buf)
{
    if (handle->handle)
    {
        char buffer[0xffff];
        if (fgets(buffer, 0xffff, (FILE *)handle->handle) != 0)
        {
            u64 length = strlen(buffer);
            *line_buf = Sere_Alloc((sizeof(char) * length) + 1, SERE_MEMORY_TAG_STRING);
            strcpy(*line_buf, buffer);
            return SERE_TRUE;
        }
    }
    return SERE_FALSE;
}

b8 Sere_FileWriteLine(Sere_FileHandle *handle, const char *text)
{
    if (handle->handle)
    {
        i32 result = fputs(text, (FILE *)handle->handle);
        if (result != EOF)
        {
            result = fputc('\n', (FILE *)handle->handle);
        }

        fflush((FILE *)handle->handle);
        return result != EOF;
    }
    return SERE_FALSE;
}

b8 Sere_FileRead(Sere_FileHandle *handle, u64 data_size, void *out_data, u64 *out_bytes_read)
{
    if (handle->handle && out_data)
    {
        *out_bytes_read = fread(out_data, 1, data_size, (FILE *)handle->handle);
        if (*out_bytes_read != data_size)
        {
            return SERE_FALSE;
        }
        return SERE_TRUE;
    }
    return SERE_FALSE;
}

b8 Sere_FileReadAllBytes(Sere_FileHandle *handle, u8 **out_bytes, u64 *out_bytes_read)
{
    if (handle->handle)
    {
        fseek((FILE *)handle->handle, 0, SEEK_END);
        u64 size = ftell((FILE *)handle->handle);
        rewind((FILE *)handle->handle);

        *out_bytes = Sere_Alloc(sizeof(u8) * size, SERE_MEMORY_TAG_STRING);
        *out_bytes_read = fread(*out_bytes, 1, size, (FILE *)handle->handle);
        if (*out_bytes_read != size)
        {
            return SERE_FALSE;
        }
        return SERE_TRUE;
    }
    return SERE_FALSE;
}

b8 Sere_FileWrite(Sere_FileHandle *handle, u64 data_size, const char *data, u64 *out_bytes_written)
{
    if (handle->handle)
    {
        *out_bytes_written = fwrite(data, 1, data_size, (FILE *)handle->handle);
        if (*out_bytes_written != data_size)
        {
            return SERE_FALSE;
        }
        fflush((FILE *)handle->handle);
        return SERE_TRUE;
    }
    return SERE_FALSE;
}
