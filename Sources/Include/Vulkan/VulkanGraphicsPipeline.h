#pragma once

#include"../InterfaceType.h"

namespace DRHI
{
	struct VulkanPipelineCreateInfo
	{
		VkShaderModule vertexShader;
		VkShaderModule fragmentShader;
	};

	void createGraphicsPipeline();
}