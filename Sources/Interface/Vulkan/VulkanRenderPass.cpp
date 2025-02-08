#include"../../Include/Vulkan/VulkanRenderPass.h"

namespace DRHI
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
			VkSubpassDependency vksd{};
			vksd.dependencyFlags = (VkDependencyFlags)ci->pDependencies->dependencyFlags;
			vksd.dstAccessMask = (VkAccessFlags)ci->pDependencies->dstAccessMask;
			vksd.dstStageMask = (VkPipelineStageFlags)ci->pDependencies->dstStageMask;
			vksd.dstSubpass = ci->pDependencies->dstSubpass;
			vksd.srcAccessMask = (VkAccessFlags)ci->pDependencies->srcAccessMask;
			vksd.srcStageMask = (VkPipelineStageFlags)ci->pDependencies->srcStageMask;
			vksd.srcSubpass = ci->pDependencies->srcSubpass;

			// color attachments
			VkAttachmentReference vkar{};
			vkar.attachment = ci->pSubpasses->pColorAttachments->attachment;
			vkar.layout = (VkImageLayout)ci->pSubpasses->pColorAttachments->layout;

			// depth attachment
			VkAttachmentReference vkdepthar{};
			vkdepthar.attachment = ci->pSubpasses->pDepthStencilAttachment->attachment;
			vkdepthar.layout = (VkImageLayout)ci->pSubpasses->pDepthStencilAttachment->layout;

			// input attachment
			VkAttachmentReference vkinputar{};
			vkinputar.attachment = ci->pSubpasses->pInputAttachments->attachment;
			vkinputar.layout = (VkImageLayout)ci->pSubpasses->pInputAttachments->layout;

			// resolve attachment
			VkAttachmentReference vkrar{};
			vkrar.attachment = ci->pSubpasses->pResolveAttachments->attachment;
			vkrar.layout = (VkImageLayout)ci->pSubpasses->pResolveAttachments->layout;

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
			vkci.attachmentCount = ci->attachmentCount;
			vkci.dependencyCount = ci->dependencyCount;
			vkci.flags = (VkRenderPassCreateFlags)ci->flags;
			vkci.pAttachments = &vkad;
			vkci.pDependencies = &vksd;
			vkci.pSubpasses = &vksdp;
			vkci.subpassCount = ci->subpassCount;

			vkCreateRenderPass(device, &vkci, nullptr, &vkpass);

			renderPass->internalID = vkpass;
		}
	}
}