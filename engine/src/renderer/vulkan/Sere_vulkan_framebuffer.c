
#include "Sere_vulkan_framebuffer.h"

#include "core/Sere_memory.h"

Sere_VulkanFramebuffer *Sere_CreateVulkanFramebuffer(
    Sere_VulkanContext *context,
    Sere_VulkanRenderPass *renderpass,
    u32 width, u32 height,
    u32 attachment_count,
    VkImageView *attachments)
{

    Sere_VulkanFramebuffer *out_framebuffer = (Sere_VulkanFramebuffer *)Sere_Alloc(sizeof(Sere_VulkanFramebuffer), SERE_MEMORY_TAG_RENDERER);

    out_framebuffer->attachments = Sere_Alloc(sizeof(VkImageView) * attachment_count, SERE_MEMORY_TAG_RENDERER);
    for (u32 i = 0; i < attachment_count; ++i)
    {
        out_framebuffer->attachments[i] = attachments[i];
    }

    out_framebuffer->renderpass = renderpass;
    out_framebuffer->attachment_count = attachment_count;

    VkFramebufferCreateInfo create_info = {VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO};
    create_info.renderPass = renderpass->handle;
    create_info.attachmentCount = attachment_count;
    create_info.pAttachments = out_framebuffer->attachments;
    create_info.width = width;
    create_info.height = height;
    create_info.layers = 1;

    SERE_VK_CHECK(vkCreateFramebuffer(
        context->device.logical_device,
        &create_info,
        context->allocator,
        &out_framebuffer->handle));

    return out_framebuffer;
}

void Sere_DestroyVulkanFramebuffer(
    Sere_VulkanContext *context,
    Sere_VulkanFramebuffer *framebuffer)
{
    vkDestroyFramebuffer(context->device.logical_device, framebuffer->handle, context->allocator);
    if (framebuffer->attachments)
    {
        Sere_Free(framebuffer->attachments, sizeof(VkImageView) * framebuffer->attachment_count, SERE_MEMORY_TAG_RENDERER);
        framebuffer->attachments = 0;
    }

    framebuffer->handle = 0;
    framebuffer->attachment_count = 0;
    framebuffer->renderpass = 0;
}
