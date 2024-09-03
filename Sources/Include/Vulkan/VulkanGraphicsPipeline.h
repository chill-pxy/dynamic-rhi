#pragma once

#include"../InterfaceType.h"

namespace DRHI
{
	struct VulkanPipelineCreateInfo
	{
		VkShaderModule vertexShader;
		VkShaderModule fragmentShader;
	};

	void createGraphicsPipeline(VkPipeline* graphicsPipeline, VkPipelineLayout* pipelineLayout, VulkanPipelineCreateInfo createInfo, VkDevice* device, VkDescriptorSetLayout* descriptorSetlayout, VkFormat* swapChainImageFormat);
}