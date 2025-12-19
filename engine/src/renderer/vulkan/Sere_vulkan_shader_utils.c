
#include "Sere_vulkan_shader_utils.h"

#include "core/Sere_string.h"
#include "core/Sere_logger.h"
#include "core/Sere_memory.h"

#include "platform/Sere_filesystem.h"

b8 Sere_CreateShaderModule(
    Sere_VulkanContext *context,
    const char *name,
    const char *type_str,
    VkShaderStageFlagBits shader_stage_flag,
    u32 stage_index,
    Sere_VulkanShaderStage *shader_stages)
{
    char file_name[1024];
    // Fix: point to bin/assets/shaders instead of ./assets/shaders
    Sere_StrFormat(file_name, sizeof(file_name),
                   "./bin/assets/shaders/%s.%s.spv", name, type_str);

    Sere_ZeroMemory(&shader_stages[stage_index].create_info,
                    sizeof(VkShaderModuleCreateInfo));
    shader_stages[stage_index].create_info.sType =
        VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;

    Sere_FileHandle *handle =
        Sere_OpenFile(file_name, SERE_FILEMODE_READ, SERE_TRUE);

    u64 size = 0;
    u8 *file_buffer = 0;
    if (!Sere_FileReadAllBytes(handle, &file_buffer, &size))
    {
        SERE_ERROR("Unable to binary read shader module: %s.", file_name);
        return SERE_FALSE;
    }

    shader_stages[stage_index].create_info.codeSize = size;
    shader_stages[stage_index].create_info.pCode = (u32 *)file_buffer;

    Sere_CloseFile(handle);

    SERE_VK_CHECK(vkCreateShaderModule(
        context->device.logical_device,
        &shader_stages[stage_index].create_info,
        0,
        &shader_stages[stage_index].handle));

    Sere_ZeroMemory(&shader_stages[stage_index].shader_stage_create_info,
                    sizeof(VkPipelineShaderStageCreateInfo));
    shader_stages[stage_index].shader_stage_create_info.sType =
        VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shader_stages[stage_index].shader_stage_create_info.stage =
        shader_stage_flag;
    shader_stages[stage_index].shader_stage_create_info.module =
        shader_stages[stage_index].handle;
    shader_stages[stage_index].shader_stage_create_info.pName = "main";

    if (file_buffer)
    {
        Sere_Free(file_buffer, sizeof(u8) * size,
                  SERE_MEMORY_TAG_STRING);
        file_buffer = 0;
    }

    return SERE_TRUE;
}
