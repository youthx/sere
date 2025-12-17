

#include "Sere_vulkan_renderpass.h"

#include "core/Sere_memory.h"

Sere_VulkanRenderPass *Sere_CreateVulkanRenderPass(
    Sere_VulkanContext *context,
    f32 x, f32 y, f32 w, f32 h,
    f32 r, f32 g, f32 b, f32 a,
    f32 depth,
    u32 stencil)
{
    
    VkSubpassDescription subpass = {};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;

    u32 attachment_description_count = 2;
    VkAttachmentDescription attachment_descs[attachment_description_count];

    VkAttachmentDescription color_attachment;
    color_attachment.format = context->swapchain.image_format.format;
    color_attachment.samples = VK_SAMPLE_COUNT_1_BIT;
    color_attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    color_attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    color_attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    color_attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    color_attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    color_attachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
    color_attachment.flags = 0;
    attachment_descs[0] = color_attachment;

    
    VkRenderPassCreateInfo render_pass_create_info = {VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO};


}

void Sere_DestroyRenderPass(
    Sere_VulkanContext *context,
    Sere_VulkanRenderPass *renderpass);

void Sere_VulkanRenderPassBegin(
    Sere_VulkanCommandBuffer *command_buffer,
    Sere_VulkanRenderPass *renderpass,
    VkFramebuffer framebuffer);

void Sere_VulkanRenderPassEnd(
    Sere_VulkanCommandBuffer *command_buffer,
    Sere_VulkanRenderPass *renderpass);
