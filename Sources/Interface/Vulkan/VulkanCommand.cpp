#include<vector>

#include"../../Include/Vulkan/VulkanCommand.h"

// Custom define for better code readability
#define VK_FLAGS_NONE 0
// Default fence timeout in nanoseconds
#define DEFAULT_FENCE_TIMEOUT 100000000000

namespace drhi
{
	namespace VulkanCommand
	{
		//https://github.com/SaschaWillems/Vulkan/blob/master/examples/dynamicrendering/dynamicrendering.cpp#L81
		void insertImageMemoryBarrier(
			VkCommandBuffer* cmdbuffer,
			VkImage* image,
			VkAccessFlags srcAccessMask,
			VkAccessFlags dstAccessMask,
			VkImageLayout oldImageLayout,
			VkImageLayout newImageLayout,
			VkPipelineStageFlags srcStageMask,
			VkPipelineStageFlags dstStageMask,
			VkImageSubresourceRange subresourceRange)
		{
			VkImageMemoryBarrier imageMemoryBarrier{};
			imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
			imageMemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			imageMemoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			imageMemoryBarrier.srcAccessMask = srcAccessMask;
			imageMemoryBarrier.dstAccessMask = dstAccessMask;
			imageMemoryBarrier.oldLayout = oldImageLayout;
			imageMemoryBarrier.newLayout = newImageLayout;
			imageMemoryBarrier.image = *image;
			imageMemoryBarrier.subresourceRange = subresourceRange;

			vkCmdPipelineBarrier(
				*cmdbuffer,
				srcStageMask,
				dstStageMask,
				0,
				0, nullptr,
				0, nullptr,
				1, &imageMemoryBarrier);
		}

		void createCommandPool(VkCommandPool* commandPool, VkDevice* device, QueueFamilyIndices queueFamilyIndices)
		{
			VkCommandPoolCreateInfo info{};
			info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
			info.pNext = NULL;
			info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
			info.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

			if (vkCreateCommandPool(*device, &info, nullptr, commandPool) != VK_SUCCESS)
			{
				throw std::runtime_error("failed to create command pool");
			}
		}

		void createCommandBuffers(std::vector<VkCommandBuffer>* commandBuffers, VkCommandPool* commandPool, VkCommandBufferLevel level, VkDevice* device)
		{
			commandBuffers->resize(MAX_FRAMES_IN_FLIGHT);

			VkCommandBufferAllocateInfo allocInfo{};
			allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
			allocInfo.commandPool = *commandPool;
			allocInfo.level = level;
			allocInfo.commandBufferCount = (uint32_t)commandBuffers->size();

			if (vkAllocateCommandBuffers(*device, &allocInfo, commandBuffers->data()) != VK_SUCCESS)
			{
				throw std::runtime_error("failed to allocate command buffers!");
			}
		}

		void createCommandBuffer(VkCommandBuffer* commandBuffer, VkCommandPool* commandPool, VkCommandBufferLevel level, VkDevice* device)
		{
			VkCommandBufferAllocateInfo allocInfo{};
			allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
			allocInfo.commandPool = *commandPool;
			allocInfo.level = level;
			allocInfo.commandBufferCount = 1;

			if (vkAllocateCommandBuffers(*device, &allocInfo, commandBuffer) != VK_SUCCESS)
			{
				throw std::runtime_error("failed to allocate command buffers!");
			}
		}

		void beginRendering(VkCommandBuffer commandBuffer, VkImage* image, VkImage* depthImage, VkImageView* imageView, VkImageView* depthImageView,
			uint32_t viewPortWidth, uint32_t viewPortHeight, bool isClear, bool includeStencil, bool isSecondaryCommand)
		{
			// New structures are used to define the attachments used in dynamic rendering
			VkRenderingAttachmentInfoKHR colorAttachment{};
			colorAttachment.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO_KHR;
			colorAttachment.imageView = *imageView;
			colorAttachment.imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
			colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
			colorAttachment.clearValue.color = { 0.0f, 0.0f, 0.0f,1.0f };

			// A single depth stencil attachment info can be used, but they can also be specified separately.
				// When both are specified separately, the only requirement is that the image view is identical.			
			VkRenderingAttachmentInfoKHR depthStencilAttachment{};
			depthStencilAttachment.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO_KHR;
			depthStencilAttachment.imageView = *depthImageView;
			depthStencilAttachment.imageLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
			depthStencilAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
			depthStencilAttachment.clearValue.depthStencil = { 1.0f,  0 };

			if (isClear)
			{
				colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
				depthStencilAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
			}
			else
			{
				colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
				depthStencilAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
			}

			VkRenderingInfoKHR renderingInfo{};
			renderingInfo.sType = VK_STRUCTURE_TYPE_RENDERING_INFO_KHR;
			if (isSecondaryCommand) renderingInfo.flags = VK_RENDERING_CONTENTS_SECONDARY_COMMAND_BUFFERS_BIT_KHR;
			renderingInfo.renderArea = { 0, 0, viewPortWidth, viewPortHeight };
			renderingInfo.layerCount = 1;
			renderingInfo.colorAttachmentCount = 1;
			renderingInfo.pColorAttachments = &colorAttachment;
			renderingInfo.pDepthAttachment = &depthStencilAttachment;
			if (includeStencil)
			{
				renderingInfo.pStencilAttachment = &depthStencilAttachment;
			}

			//Begin dynamic rendering
			vkCmdBeginRenderingKHR(commandBuffer, &renderingInfo);

			VkViewport viewport{};
			viewport.width = viewPortWidth;
			viewport.height = viewPortHeight;
			viewport.minDepth = 0.0f;
			viewport.maxDepth = 1.0f;

			vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

			VkRect2D scissor{};
			scissor.extent.width = viewPortWidth;
			scissor.extent.height = viewPortHeight;
			scissor.offset.x = 0;
			scissor.offset.y = 0;

			vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
		}

		void beginRendering(VkCommandBuffer commandBuffer, VkImage* image, VkImageView* imageView,
			uint32_t viewPortWidth, uint32_t viewPortHeight, bool isClear, bool isSecondaryCommand)
		{
			// New structures are used to define the attachments used in dynamic rendering
			VkRenderingAttachmentInfoKHR colorAttachment{};
			colorAttachment.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO_KHR;
			colorAttachment.imageView = *imageView;
			colorAttachment.imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
			colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
			colorAttachment.clearValue.color = { 0.0f, 0.0f, 0.0f,1.0f };

			if (isClear)
			{
				colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
			}
			else
			{
				colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
			}

			VkRenderingInfoKHR renderingInfo{};
			renderingInfo.sType = VK_STRUCTURE_TYPE_RENDERING_INFO_KHR;
			if (isSecondaryCommand) renderingInfo.flags = VK_RENDERING_CONTENTS_SECONDARY_COMMAND_BUFFERS_BIT_KHR;
			renderingInfo.renderArea = { 0, 0, viewPortWidth, viewPortHeight };
			renderingInfo.layerCount = 1;
			renderingInfo.colorAttachmentCount = 1;
			renderingInfo.pColorAttachments = &colorAttachment;

			//Begin dynamic rendering
			vkCmdBeginRenderingKHR(commandBuffer, &renderingInfo);

			VkViewport viewport{};
			viewport.width = viewPortWidth;
			viewport.height = viewPortHeight;
			viewport.minDepth = 0.0f;
			viewport.maxDepth = 1.0f;

			vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

			VkRect2D scissor{};
			scissor.extent.width = viewPortWidth;
			scissor.extent.height = viewPortHeight;
			scissor.offset.x = 0;
			scissor.offset.y = 0;

			vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
		}

		void beginRendering(VkCommandBuffer commandBuffer, std::vector<VkImageView>& imageViews,
			VkImage* depthImage, VkImageView* depthImageView, uint32_t viewPortWidth, uint32_t viewPortHeight, bool isClear, bool includeStencil, bool isSecondaryCommand)
		{
			// depth attachment
			VkRenderingAttachmentInfoKHR depthStencilAttachment{};
			depthStencilAttachment.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO_KHR;
			depthStencilAttachment.imageView = *depthImageView;
			depthStencilAttachment.imageLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
			depthStencilAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
			depthStencilAttachment.clearValue.depthStencil = { 1.0f,  0 };

			// New structures are used to define the attachments used in dynamic rendering
			std::vector<VkRenderingAttachmentInfoKHR> colorAttachments{};
			colorAttachments.resize(imageViews.size());
			for (uint32_t i = 0; i < imageViews.size(); ++i)
			{
				colorAttachments[i].sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO_KHR;
				colorAttachments[i].imageView = imageViews[i];
				colorAttachments[i].imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
				colorAttachments[i].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
				colorAttachments[i].clearValue.color = {0.0f, 0.0f, 0.0f,1.0f};
				if (isClear)
				{
					colorAttachments[i].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
					depthStencilAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
				}
				else
				{
					colorAttachments[i].loadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
					depthStencilAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
				}
			}

			VkRenderingInfoKHR renderingInfo{};
			renderingInfo.sType = VK_STRUCTURE_TYPE_RENDERING_INFO_KHR;
			if (isSecondaryCommand) renderingInfo.flags = VK_RENDERING_CONTENTS_SECONDARY_COMMAND_BUFFERS_BIT_KHR;
			renderingInfo.renderArea = { 0, 0, viewPortWidth, viewPortHeight };
			renderingInfo.layerCount = 1;
			renderingInfo.colorAttachmentCount = imageViews.size();
			renderingInfo.pColorAttachments = colorAttachments.data();
			renderingInfo.pDepthAttachment = &depthStencilAttachment;
			if (includeStencil)
			{
				renderingInfo.pStencilAttachment = &depthStencilAttachment;
			}

			//Begin dynamic rendering
			vkCmdBeginRenderingKHR(commandBuffer, &renderingInfo);

			VkViewport viewport{};
			viewport.width = viewPortWidth;
			viewport.height = viewPortHeight;
			viewport.minDepth = 0.0f;
			viewport.maxDepth = 1.0f;

			vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

			VkRect2D scissor{};
			scissor.extent.width = viewPortWidth;
			scissor.extent.height = viewPortHeight;
			scissor.offset.x = 0;
			scissor.offset.y = 0;

			vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
		}

		VkCommandBuffer beginSingleTimeCommands(VkCommandPool* commandPool, VkDevice* device)
		{
			VkCommandBufferAllocateInfo allocInfo{};
			allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
			allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
			allocInfo.commandPool = *commandPool;
			allocInfo.commandBufferCount = 1;

			VkCommandBuffer commandBuffer;
			vkAllocateCommandBuffers(*device, &allocInfo, &commandBuffer);

			VkCommandBufferBeginInfo beginInfo{};
			beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
			beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

			vkBeginCommandBuffer(commandBuffer, &beginInfo);

			return commandBuffer;
		}

		void endSingleTimeCommands(VkCommandBuffer commandBuffer, VkQueue* graphicsQueue, VkCommandPool* commandPool, VkDevice* device)
		{
			vkEndCommandBuffer(commandBuffer);

			VkSubmitInfo submitInfo{};
			submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
			submitInfo.commandBufferCount = 1;
			submitInfo.pCommandBuffers = &commandBuffer;

			vkQueueSubmit(*graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
			vkQueueWaitIdle(*graphicsQueue);

			vkFreeCommandBuffers(*device, *commandPool, 1, &commandBuffer);
		}

		void flushCommandBuffer(VkDevice device, VkCommandBuffer commandBuffer, VkQueue queue, VkCommandPool pool, bool free)
		{
			if (commandBuffer == VK_NULL_HANDLE)
			{
				return;
			}

			vkEndCommandBuffer(commandBuffer);

			VkSubmitInfo submitInfo{};
			submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
			submitInfo.commandBufferCount = 1;
			submitInfo.pCommandBuffers = &commandBuffer;
			// Create fence to ensure that the command buffer has finished executing
			VkFenceCreateInfo fenceCreateInfo{};
			fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
			fenceCreateInfo.flags = VK_FLAGS_NONE;
			VkFence fence;
			vkCreateFence(device, &fenceCreateInfo, nullptr, &fence);
			// Submit to the queue
			vkQueueSubmit(queue, 1, &submitInfo, fence);
			// Wait for the fence to signal that command buffer has finished executing
			vkWaitForFences(device, 1, &fence, VK_TRUE, DEFAULT_FENCE_TIMEOUT);
			vkDestroyFence(device, fence, nullptr);
			if (free)
			{
				vkFreeCommandBuffers(device, pool, 1, &commandBuffer);
			}
		}
	}
}