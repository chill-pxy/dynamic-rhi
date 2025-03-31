#pragma once

#include<stdexcept>

#include "../../Include/InterfaceType.h"
#include "VulkanDevice.h"

namespace drhi
{
	namespace VulkanCommand
	{
		void insertImageMemoryBarrier(
			VkCommandBuffer* cmdbuffer,
			VkImage* image,
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
			uint32_t viewPortWidth, uint32_t viewPortHeight, bool isClear, bool includeStencil);

		void beginRendering(VkCommandBuffer commandBuffer, VkImage* swapchainImage, VkImageView* swapchainImageView,
			uint32_t viewPortWidth, uint32_t viewPortHeight, bool isClear);

		VkCommandBuffer beginSingleTimeCommands(VkCommandPool* commandPool, VkDevice* device);

		void endSingleTimeCommands(VkCommandBuffer commandBuffer, VkQueue* graphicsQueue, VkCommandPool* commandPool, VkDevice* device);
	
		void flushCommandBuffer(VkDevice device, VkCommandBuffer commandBuffer, VkQueue queue, VkCommandPool pool, bool free);
	}
}