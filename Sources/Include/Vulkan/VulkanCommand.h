#pragma once

#include<stdexcept>

#include "../../Include/InterfaceType.h"
#include "VulkanDevice.h"

namespace DRHI
{
	namespace VulkanCommand
	{
		void insertImageMemoryBarrier(
			VkCommandBuffer cmdbuffer,
			VkImage image,
			VkAccessFlags srcAccessMask,
			VkAccessFlags dstAccessMask,
			VkImageLayout oldImageLayout,
			VkImageLayout newImageLayout,
			VkPipelineStageFlags srcStageMask,
			VkPipelineStageFlags dstStageMask,
			VkImageSubresourceRange subresourceRange);

		void createCommandPool(VkCommandPool* commandPool, VkDevice* device, QueueFamilyIndices queueFamilyIndices);

		void createCommandBuffers(std::vector<VkCommandBuffer>* commandBuffers, VkCommandPool* commandPool, VkCommandBufferLevel level, VkDevice* device);

		void createCommandBuffer(VkCommandBuffer* commandBuffer, VkCommandPool* commandPool, VkCommandBufferLevel level, VkDevice* device);

		void beginRendering(VkCommandBuffer commandBuffer, VkImage* swapchainImage, VkImage* depthImage, VkImageView* swapchainImageView, VkImageView* depthImageView,
			uint32_t viewPortWidth, uint32_t viewPortHeight, bool isClear);

		VkCommandBuffer beginSingleTimeCommands(VkCommandPool* commandPool, VkDevice* device);

		void endSingleTimeCommands(VkCommandBuffer commandBuffer, VkQueue* graphicsQueue, VkCommandPool* commandPool, VkDevice* device);
	}
}