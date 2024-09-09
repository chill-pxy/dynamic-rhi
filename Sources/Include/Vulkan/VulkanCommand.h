#pragma once

#include<stdexcept>

#include "../../Include/InterfaceType.h"
#include "VulkanDevice.h"

namespace DRHI
{
	void createCommandPool(VkCommandPool* commandPool, VkDevice* device, QueueFamilyIndices queueFamilyIndices);

	void createCommandBuffers(std::vector<VkCommandBuffer>* commandBuffers, VkCommandPool* commandPool, VkDevice* device);

	VkCommandBuffer beginSingleTimeCommands(VkCommandPool* commandPool, VkDevice* device);

	void endSingleTimeCommands(VkCommandBuffer commandBuffer, VkQueue* graphicsQueue, VkCommandPool* commandPool, VkDevice* device);
}