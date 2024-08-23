#pragma once

#include<variant>
#include<iostream>

#include"InterfaceType.h"
#include"SwapChain.h"
#include"Device.h"
#include"PhysicalDevice.h"

namespace DRHI
{
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