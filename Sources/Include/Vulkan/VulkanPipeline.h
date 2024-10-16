#pragma once

#include<vector>

#include"../InterfaceType.h"

namespace DRHI
{
	namespace VulkanPipeline
	{
		struct VulkanPipelineCreateInfo
		{
			VkShaderModule vertexShader;
			VkShaderModule fragmentShader;
		};

		void createPipelineLayout(DynamicPipelineLayout* pipelineLayout ,DynamicPipelineLayoutCreateInfo* createInfo, VkDevice* device);

		void createGraphicsPipeline(VkPipeline* graphicsPipeline, VkPipelineLayout* pipelineLayout, VkPipelineCache* pipelineCache, VulkanPipelineCreateInfo createInfo, VkDevice* device, VkFormat* swapChainImageFormat, VkVertexInputBindingDescription bindingDescription, std::vector<VkVertexInputAttributeDescription> attributeDescriptions);

		void createPipelineCache(VkPipelineCache* pipelineCache, VkDevice* device);

		VkPipelineRenderingCreateInfoKHR getPipelineRenderingCreateInfo(VkFormat* swapChainImageFormat);
	}
}