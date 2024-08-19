#pragma once

#include<variant>
#include<iostream>

#include"InterfaceType.h"
#include"ShaderModule.h"
#include"Device.h"
#include"DescriptorPool.h"
#include"RenderPass.h"

namespace DRHI
{
	struct PipelineCreateInfo
	{
		ShaderModule vertexShader;
		ShaderModule fragmentShader;
	};

	class GraphicsPipeline
	{
	private:
		std::variant<VkPipeline*> _runtimeGraphicsPipeline;
		std::variant<VkPipelineLayout*> _runtimePipelineLayout;

	public:
		void createGraphicsPipeline(PipelineCreateInfo createInfo, Device* pdevice, DescriptorPool* pdescriptorPool, RenderPass* prenderPass);

	public:
		GraphicsPipeline()
		{
			_runtimeGraphicsPipeline = new VkPipeline();
			_runtimePipelineLayout = new VkPipelineLayout();
		}

		GraphicsPipeline(API api)
		{
			switch (api)
			{
			case VULKAN:
				_runtimeGraphicsPipeline = new VkPipeline();
				_runtimePipelineLayout = new VkPipelineLayout();
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

		VkPipelineLayout* getPipelineLayout()
		{
			if (std::holds_alternative<VkPipelineLayout*>(_runtimePipelineLayout))
			{
				return std::get<VkPipelineLayout*>(_runtimePipelineLayout));
			}
			else
			{
				std::cout << "none vk pipeline layout";
			}
		}
	};
}