#include"../../Include/Vulkan/VulkanFramebuffer.h"

namespace DRHI
{
	namespace VulkanFramebuffer
	{
		void createFramebuffer(DynamicFramebuffer* framebuffer, DynamicFramebufferCreateInfo* fci, VkDevice device)
		{
			VkFramebuffer vkfb{};

			VkImageView attachment = fci->pAttachments->getVulkanImageView();

			VkFramebufferCreateInfo vkci{};
			vkci.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			vkci.attachmentCount = fci->attachmentCount;
			vkci.flags = (VkFramebufferCreateFlags)fci->flags;
			vkci.width = fci->width;
			vkci.height = fci->height;
			vkci.layers = fci->layers;
			vkci.pAttachments = &attachment;
			vkci.renderPass = fci->renderPass.getVulkanRenderPass();

			vkCreateFramebuffer(device, &vkci, nullptr, &vkfb);

			framebuffer->internalID = vkfb;
		}
	}
}