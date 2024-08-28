#pragma once

#include<variant>
#include<iostream>

#include"InterfaceType.h"
#include"SwapChain.h"
#include"Device.h"
#include"PhysicalDevice.h"

namespace DRHI
{
	/*typedef struct SubpassDependency
	{
		uint32_t                srcSubpass;
		uint32_t                dstSubpass;
		PipelineStageFlags    srcStageMask;
		PipelineStageFlags    dstStageMask;
		AccessFlags           srcAccessMask;
		AccessFlags           dstAccessMask;
		DependencyFlags       dependencyFlags;
	} SubpassDependency;

	typedef struct SubpassDescription
	{
		SubpassDescriptionFlags       flags;
		PipelineBindPoint             pipelineBindPoint;
		uint32_t                        inputAttachmentCount;
		const AttachmentReference* pInputAttachments;
		uint32_t                        colorAttachmentCount;
		const AttachmentReference* pColorAttachments;
		const AttachmentReference* pResolveAttachments;
		const AttachmentReference* pDepthStencilAttachment;
		uint32_t                        preserveAttachmentCount;
		const uint32_t* pPreserveAttachments;
	}SubpassDescription;

	typedef struct AttachmentReference
	{
		uint32_t         attachment;
		ImageLayout    layout;
	} AttachmentReference;

	typedef struct AttachmentDescription
	{
		AttachmentDescriptionFlags    flags;
		Format                        format;
		SampleCountFlagBits           samples;
		AttachmentLoadOp              loadOp;
		AttachmentStoreOp             storeOp;
		AttachmentLoadOp              stencilLoadOp;
		AttachmentStoreOp             stencilStoreOp;
		ImageLayout                   initialLayout;
		ImageLayout                   finalLayout;
	} AttachmentDescription;
	 
	typedef struct RenderPassCreateInfo
	{
		AttachmentDescription attachmentDescription;
		AttachmentReference attachmentReference;
		SubpassDescription subpassDescription;
		SubpassDependency subpassDependency;
	}RenderPassCreateInfo;*/

	class RenderPass
	{
	private:
		std::variant<VkRenderPass*> _runtimeRenderPass;

	public:
		void createRenderPass(SwapChain* pswapChain, Device* pdevice, PhysicalDevice* pphysicalDeice);

	public:
		RenderPass()
		{
			_runtimeRenderPass = new VkRenderPass();
		}

		RenderPass(API api)
		{
			switch (api)
			{
			case VULKAN:
				_runtimeRenderPass = new VkRenderPass();
				break;
			case DIRECT3D12:
				break;
			default:
				break;
			}
		}

		VkRenderPass* getVkRenderPass()
		{
			if (std::holds_alternative<VkRenderPass*>(_runtimeRenderPass))
			{
				return std::get<VkRenderPass*>(_runtimeRenderPass);
			}
			else
			{
				std::cout << "none vk render pass";

				return nullptr;
			}
		}
		
		template<typename T>
		void setRuntimeRenderPass(T renderPass)
		{
			_runtimeRenderPass = renderPass;
		}
	};
}