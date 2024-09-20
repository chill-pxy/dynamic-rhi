#pragma once

#include<vector>

#include"../InterfaceType.h"

namespace DRHI
{
	struct VulkanPipelineCreateInfo
	{
		VkShaderModule vertexShader;
		VkShaderModule fragmentShader;
	};

	void createGraphicsPipeline(VkPipeline* graphicsPipeline, VkPipelineLayout* pipelineLayout, VkPipelineCache* pipelineCache, VulkanPipelineCreateInfo createInfo, VkDevice* device, VkDescriptorSetLayout* descriptorSetlayout, VkFormat* swapChainImageFormat, VkVertexInputBindingDescription bindingDescription, std::vector<VkVertexInputAttributeDescription> attributeDescriptions);

	void createPipelineCache(VkPipelineCache* pipelineCache, VkDevice* device);
}