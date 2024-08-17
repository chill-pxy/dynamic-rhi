#pragma once

#include<variant>
#include<iostream>

#include"InterfaceType.h"
#include"ShaderModule.h"
#include"Device.h"

namespace DRHI
{
	struct PipelineCreateInfo
	{
		ShaderModule shaders[2];
	};

	class GraphicsPipeline
	{
	private:
		std::variant<VkPipeline*> _runtimeGraphicsPipeline;

	public:
		void createGraphicsPipeline(PipelineCreateInfo createInfo, Device* pdevice);

	public:
		GraphicsPipeline()
		{
			_runtimeGraphicsPipeline = new VkPipeline();
		}

		GraphicsPipeline(API api)
		{
			switch (api)
			{
			case VULKAN:
				_runtimeGraphicsPipeline = new VkPipeline();
				break;
			case DIRECT3D12:
				break;
			default:
				break;
			}
		}

		VkPipeline* getGraphicsPipeline()
		{
			if (std::holds_alternative<VkPipeline*>(_runtimeGraphicsPipeline))
			{
				return std::get<VkPipeline*>(_runtimeGraphicsPipeline);
			}
			else
			{
				std::cout << "none vk graphics pipeline";
			}
		}
	};
}