#include"../../Include/Vulkan/VulkanRenderPass.h"

namespace drhi
{
	namespace VulkanRenderPass
	{
		void createRenderPass(DynamicRenderPass* renderPass, DynamicRenderPassCreateInfo* ci, VkDevice device)
		{
			VkRenderPass vkpass{};

			// attachment description
			VkAttachmentDescription vkad{};
			vkad.finalLayout = (VkImageLayout)ci->pAttachments->finalLayout;
			vkad.flags = (VkAttachmentDescriptionFlags)ci->pAttachments->flags;
			vkad.format = (VkFormat)ci->pAttachments->format;
			vkad.initialLayout = (VkImageLayout)ci->pAttachments->initialLayout;
			vkad.loadOp = (VkAttachmentLoadOp)ci->pAttachments->loadOp;
			vkad.samples = (VkSampleCountFlagBits)ci->pAttachments->samples;
			vkad.stencilLoadOp = (VkAttachmentLoadOp)ci->pAttachments->stencilLoadOp;
			vkad.stencilStoreOp = (VkAttachmentStoreOp)ci->pAttachments->stencilStoreOp;
			vkad.storeOp = (VkAttachmentStoreOp)ci->pAttachments->storeOp;

			// subpass dependency
			std::vector<VkSubpassDependency> vksds{};
			vksds.resize(ci->dependencyCount);
			
			int index = 0;
			for (auto v : *ci->pDependencies)
			{
				vksds[index].dependencyFlags = (VkDependencyFlags)v.dependencyFlags;
				vksds[index].dstAccessMask = (VkAccessFlags)v.dstAccessMask;
				vksds[index].dstStageMask = (VkPipelineStageFlags)v.dstStageMask;
				vksds[index].dstSubpass = v.dstSubpass;
				vksds[index].srcAccessMask = (VkAccessFlags)v.srcAccessMask;
				vksds[index].srcStageMask = (VkPipelineStageFlags)v.srcStageMask;
				vksds[index].srcSubpass = v.srcSubpass;
				index++;
			}

			// color attachments
			VkAttachmentReference vkar{};
			if (ci->pSubpasses->pColorAttachments)
			{
				vkar.attachment = ci->pSubpasses->pColorAttachments->attachment;
				vkar.layout = (VkImageLayout)ci->pSubpasses->pColorAttachments->layout;
			}
			else
			{
				vkar.attachment = VK_ATTACHMENT_UNUSED;
				vkar.layout = VK_IMAGE_LAYOUT_UNDEFINED;
			}

			// depth attachment
			VkAttachmentReference vkdepthar{};
			if (ci->pSubpasses->pDepthStencilAttachment) 
			{
				vkdepthar.attachment = ci->pSubpasses->pDepthStencilAttachment->attachment;
				vkdepthar.layout = (VkImageLayout)ci->pSubpasses->pDepthStencilAttachment->layout;
			}
			else
			{
				vkdepthar.attachment = VK_ATTACHMENT_UNUSED;
				vkdepthar.layout = VK_IMAGE_LAYOUT_UNDEFINED;
			}

			// input attachment
			VkAttachmentReference vkinputar{};
			if (ci->pSubpasses->pInputAttachments)
			{
				vkinputar.attachment = ci->pSubpasses->pInputAttachments->attachment;
				vkinputar.layout = (VkImageLayout)ci->pSubpasses->pInputAttachments->layout;
			}
			else
			{
				vkinputar.attachment = VK_ATTACHMENT_UNUSED;
				vkinputar.layout = VK_IMAGE_LAYOUT_UNDEFINED;
			}

			// resolve attachment
			VkAttachmentReference vkrar{};
			if (ci->pSubpasses->pResolveAttachments)
			{
				vkrar.attachment = ci->pSubpasses->pResolveAttachments->attachment;
				vkrar.layout = (VkImageLayout)ci->pSubpasses->pResolveAttachments->layout;
			}
			else
			{
				vkrar.attachment = VK_ATTACHMENT_UNUSED;
				vkrar.layout = VK_IMAGE_LAYOUT_UNDEFINED;
			}

			// subpass description
			VkSubpassDescription vksdp{};
			vksdp.colorAttachmentCount = ci->pSubpasses->colorAttachmentCount;
			vksdp.flags = ci->pSubpasses->flags;
			vksdp.inputAttachmentCount = ci->pSubpasses->inputAttachmentCount;
			vksdp.pColorAttachments = &vkar;
			vksdp.pDepthStencilAttachment = &vkdepthar;
			vksdp.pInputAttachments = &vkinputar;
			vksdp.pipelineBindPoint = (VkPipelineBindPoint)ci->pSubpasses->pipelineBindPoint;
			vksdp.pPreserveAttachments = ci->pSubpasses->pPreserveAttachments;
			vksdp.preserveAttachmentCount = ci->pSubpasses->preserveAttachmentCount;
			vksdp.pResolveAttachments = &vkrar;
			
			VkRenderPassCreateInfo vkci{};
			vkci.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
			vkci.attachmentCount = ci->attachmentCount;
			vkci.dependencyCount = ci->dependencyCount;
			vkci.flags = (VkRenderPassCreateFlags)ci->flags;
			vkci.pAttachments = &vkad;
			vkci.pDependencies = vksds.data();
			vkci.pSubpasses = &vksdp;
			vkci.subpassCount = ci->subpassCount;

			vkCreateRenderPass(device, &vkci, nullptr, &vkpass);

			renderPass->internalID = vkpass;
		}
	}
}