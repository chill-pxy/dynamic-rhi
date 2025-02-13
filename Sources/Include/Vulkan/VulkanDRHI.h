#pragma once

#include<vector>
#include<functional>

#include "../DynamicRHI.h"
#include "VulkanInstance.h"
#include "VulkanSurface.h"
#include "VulkanPhysicalDevice.h"
#include "VulkanDevice.h"
#include "VulkanQueueFamily.h"
#include "VulkanSwapChain.h"
#include "VulkanCommand.h"
#include "VulkanDescriptor.h"
#include "VulkanPipeline.h"
#include "VulkanSemphores.h"
#include "VulkanBuffer.h"
#include "VulkanFence.h"
#include "VulkanImage.h"
#include "VulkanRenderPass.h"
#include "VulkanFramebuffer.h"

namespace DRHI
{
	struct RHICreateInfo
	{
		PlatformInfo platformInfo;
	};

	class VulkanDRHI : public DynamicRHI
	{	
	public:
		VkInstance                   _instance{ VK_NULL_HANDLE };
		VkSurfaceKHR                 _surface{ VK_NULL_HANDLE };

		VkPhysicalDevice             _physicalDevice{ VK_NULL_HANDLE };
		VkDevice                     _device{ VK_NULL_HANDLE };

		QueueFamilyIndices           _queueFamilyIndices;
		VkQueue                      _graphicQueue{ VK_NULL_HANDLE };
		VkQueue                      _presentQueue{ VK_NULL_HANDLE };

		VkSwapchainKHR               _swapChain{ VK_NULL_HANDLE };
		VkFormat                     _depthFormat{ VK_FORMAT_D32_SFLOAT_S8_UINT };
		DepthStencil                 _depthStencil{};
		VkFormat                     _swapChainImageFormat{ VK_FORMAT_B8G8R8A8_UNORM };
		VkExtent2D                   _swapChainExtent{ 0 };
		std::vector<VkImage>         _swapChainImages;
		std::vector<VkImageView>     _swapChainImageViews;
		std::vector<VkFramebuffer>   _swapChainFramebuffers;

		VkPipelineCache              _pipelineCache{ VK_NULL_HANDLE };
		PlatformInfo                 _platformInfo{};

		Semaphores                   _semaphores{ VK_NULL_HANDLE, VK_NULL_HANDLE };
		VkPipelineStageFlags         _submitPipelineStages{ VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
		
		uint32_t                     _viewPortWidth{ 0 };
		uint32_t                     _viewPortHeight{ 0 };
		std::vector<VkFence>         _waitFences;

		bool _prepare{ false };
		bool _waitForRendering{ false };
		bool _lastWaitingState{ false };

		// Active frame index
		uint32_t _currentFrame = 0;
		// Contains command buffers and semaphores to be presented to the queue
		VkSubmitInfo _submitInfo{};

	public:
		VulkanDRHI() = delete;
		VulkanDRHI(RHICreateInfo createInfo);
		
		//-------------------------------------------------------------------------------------------------------------------------- 
		//--------------------------------------------------------------------------------------------------------------------------  
		//--------------------------------------------------------public interfaces-------------------------------------------------
		//-------------------------------------------------------------------------------------------------------------------------- 
		//--------------------------------------------------------------------------------------------------------------------------  
		
		//get current api type
		virtual API getCurrentAPI() { return API::VULKAN; };

		//initialize function
		virtual void initialize();

		//tick function
		virtual void frameOnTick(std::vector<std::function<void()>> recreatefuncs, std::vector<DynamicCommandBuffer>* commandBuffers);

		//draw function
		virtual void drawIndexed(DynamicCommandBuffer* commandBuffer, uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, uint32_t vertexOffset, uint32_t firstInstance);

		//scissor
		virtual void setScissor(DynamicCommandBuffer* commandBuffer, uint32_t firstScissor, uint32_t scissorCount, DynamicRect2D rect);

		//clear functions
		virtual void clean();

		//get frame
		virtual uint32_t getCurrentFrame();

		//swap chain
		virtual uint32_t getSwapChainExtentWidth();
		virtual uint32_t getSwapChainExtentHeight();

		//command functions
		virtual void createCommandPool(DynamicCommandPool* commandPool);
		virtual void createCommandBuffers(std::vector<DynamicCommandBuffer>* commandBuffers, DynamicCommandPool* commandPool);
		virtual void createCommandBuffer(DynamicCommandBuffer* commandBuffer, DynamicCommandPool* commandPool);
		virtual void beginCommandBuffer(DynamicCommandBuffer commandBuffer);
		virtual void beginRendering(DynamicCommandBuffer commandBuffer, DynamicRenderingInfo bri);
		virtual void endCommandBuffer(DynamicCommandBuffer commandBuffer);
		virtual void endRendering(DynamicCommandBuffer commandBuffer, DynamicRenderingInfo bri);
		virtual void freeCommandBuffers(std::vector<DynamicCommandBuffer>* commandBuffers, DynamicCommandPool* commandPool);
		virtual void destroyCommandPool(DynamicCommandPool* commandPool);
		virtual void flushCommandBuffer(DynamicCommandBuffer cmdBuf, DynamicCommandPool cmdPool, bool free);

		//buffer functions
		virtual void bindVertexBuffers(DynamicBuffer* vertexBuffer, DynamicCommandBuffer* commandBuffer);
		virtual void bindIndexBuffer(DynamicBuffer* indexBuffer, DynamicCommandBuffer* commandBuffer, uint32_t indexType);
		virtual void createDynamicBuffer(DynamicBuffer* buffer, DynamicDeviceMemory* deviceMemory, DynamicCommandPool* commandPool, uint64_t bufferSize, void* bufferData, uint32_t usage, uint32_t memoryProperty);
		virtual void createUniformBuffer(std::vector<DynamicBuffer>* uniformBuffers, std::vector<DynamicDeviceMemory>* uniformBuffersMemory, std::vector<void*>* uniformBuffersMapped, uint32_t bufferSize);
		virtual void createUniformBuffer(DynamicBuffer* uniformBuffer, DynamicDeviceMemory* uniformBufferMemory, void** uniformBufferMapped, uint32_t bufferSize);
		//virtual void createDynamicBuffer(DynamicBuffer* buffer, DynamicDeviceMemory* memory, uint64_t bufferSize, uint32_t usage);
		virtual void clearBuffer(DynamicBuffer* buffer, DynamicDeviceMemory* memory);
		virtual void flushBuffer(DynamicDeviceMemory* memory, uint32_t size, uint32_t offset);	
		virtual void flushBuffer(DynamicDeviceMemory* memory, uint32_t offset);

		//memory functions
		virtual void mapMemory(DynamicDeviceMemory* memory, uint32_t offset, uint32_t size, void* data);
		virtual void unmapMemory(DynamicDeviceMemory* memory);

		//descriptor funcions
		virtual void createDescriptorPool(DynamicDescriptorPool* descriptorPool, std::vector<DynamicDescriptorPoolSize>* poolsizes);
		virtual void createDescriptorPool(DynamicDescriptorPool* descriptorPool);
		virtual void createDescriptorPool(DynamicDescriptorPool* descriptorPool, DynamicDescriptorPoolCreateInfo* ci);
		virtual void createDescriptorSet(DynamicDescriptorSet* descriptorSet, DynamicDescriptorSetLayout* descriptorSetLayout, DynamicDescriptorPool* descriptorPool, std::vector<DynamicWriteDescriptorSet>* wds, uint32_t imageCount);
		virtual void bindDescriptorSets(DynamicDescriptorSet* descriptorSet, DynamicPipelineLayout pipelineLayout, DynamicCommandBuffer* commandBuffer, uint32_t bindPoint);
		virtual void createDescriptorSetLayout(DynamicDescriptorSetLayout* descriptorSetLayout, std::vector<DynamicDescriptorSetLayoutBinding>* dsbs);
		virtual void clearDescriptorPool(DynamicDescriptorPool* descriptorPool);
		virtual void clearDescriptorSetLayout(DynamicDescriptorSetLayout* descriptorSetLayout);
		virtual void freeDescriptorSets(DynamicDescriptorSet* descriptorSet, DynamicDescriptorPool* descriptorPool);

		//texture funcitons
		virtual void createTextureImage(DynamicImage* textureImage, DynamicDeviceMemory* textureMemory, DynamicCommandPool* commandPool, int texWidth, int texHeight, int texChannels, stbi_uc* pixels);
		virtual void createTextureSampler(DynamicSampler* textureSampler);

		//image functions
		virtual void createImageView(DynamicImageView* imageView, DynamicImage* image, uint32_t imageFormat, uint32_t imageAspect);
		virtual void createImageView(DynamicImageView* imageView, DynamicImage* image, DynamicImageViewCreateInfo info);
		virtual void createImage(DynamicImage* image, uint32_t width, uint32_t height,
			uint32_t format, uint32_t imageTiling, uint32_t imageUsageFlagBits, uint32_t sampleCounts, uint32_t memoryPropertyFlags, DynamicDeviceMemory* imageMemory);
		virtual void createImage(DynamicImage* image, DynamicDeviceMemory* imageMemory, DynamicImageCreateInfo info, uint32_t memoryPropertyFlags);
		virtual void copyBufferToImage(DynamicBuffer* buffer, DynamicImage* image, DynamicCommandPool* commandPool, uint32_t width, uint32_t height);
		virtual void createSampler(DynamicSampler* sampler, DynamicSamplerCreateInfo createInfo);
		virtual void clearImage(DynamicImageView* imageView, DynamicImage* image, DynamicDeviceMemory* memory);
		virtual void clearSampler(DynamicSampler* sampler);
		virtual void createViewportImage(std::vector<DynamicImage>* viewportImages, std::vector<DynamicDeviceMemory>* viewportImageMemorys, DynamicCommandPool* commandPool);
		virtual void createViewportImageViews(std::vector<DynamicImageView>* viewportImageViews, std::vector<DynamicImage>* viewportImages);
		virtual void transitionImageLayout(DynamicImage* image, DynamicCommandPool* commandPool, uint32_t format, uint32_t oldLayout, uint32_t newLayout);
		virtual void createDepthStencil(DynamicImage* depthImage, DynamicImageView* depthImageView, DynamicDeviceMemory* memory, uint32_t format, uint32_t width, uint32_t height, uint32_t sampleCounts);
		virtual void setImageLayout(DynamicCommandBuffer* cmdBuf, DynamicImage* image, uint32_t aspectMask, uint32_t oldImageLayout, uint32_t newImageLayout);
		virtual void setImageLayout(DynamicCommandBuffer* cmdBuf, DynamicImage* image, uint32_t oldImageLayout, uint32_t newImageLayout, DynamicImageSubresourceRange range);

		//pipeline functions
		virtual void createPipelineLayout(DynamicPipelineLayout* pipelineLayout, DynamicPipelineLayoutCreateInfo* createInfo);
		virtual void createPipeline(DynamicPipeline* pipeline, DynamicPipelineLayout* pipelineLayout, DynamicPipelineCreateInfo info);
		virtual void bindPipeline(DynamicPipeline pipeline, DynamicCommandBuffer* commandBuffer, uint32_t bindPoint);
		        VkPipelineRenderingCreateInfoKHR getPipelineRenderingCreateInfo();
		virtual void clearPipeline(DynamicPipeline* pipeline, DynamicPipelineLayout* pipelineLayout);
		
		//cmd functions
		virtual void cmdPushConstants(DynamicPipelineLayout* layout, DynamicCommandBuffer* commandBuffer, uint32_t stage, uint32_t offset, uint32_t size, void* value);
		virtual void cmdSetDepthBias(DynamicCommandBuffer commandBuffer, float depthBias, float depthBiasClamp, float depthBiasSlope);
		virtual void cmdSetViewport(DynamicCommandBuffer cmdBuf, uint32_t firstViewport, uint32_t viewportCount, DynamicViewport viewport);
		virtual void cmdSetScissor(DynamicCommandBuffer cmdBuf, uint32_t firstScissor, uint32_t scissorCount, DynamicRect2D scissor);
		virtual void cmdDraw(DynamicCommandBuffer cmdBuf, uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance);
		virtual void cmdQueueWaitIdle();

		//render pass functions
		virtual void createRenderPass(DynamicRenderPass* renderPass, DynamicRenderPassCreateInfo* createInfo);
		virtual void beginRenderPass(DynamicCommandBuffer* cmdBuffer, DynamicRenderPassBeginInfo* info, uint32_t subpassContents);
		virtual void endRenderPass(DynamicCommandBuffer* cmdBuffer);
		virtual void clearRenderPass(DynamicRenderPass* renderPass);

		//framebuffer functions
		virtual void createFramebuffer(DynamicFramebuffer* frameBuffer, DynamicFramebufferCreateInfo* createInfo);
		virtual void clearFramebuffer(DynamicFramebuffer* frameBuffer);

		//-------------------------------------------------------------------------------------------------------------------------- 
		//--------------------------------------------------------------------------------------------------------------------------  
		//--------------------------------------------------------------------------------------------------------------------------
		//-------------------------------------------------------------------------------------------------------------------------- 
		//--------------------------------------------------------------------------------------------------------------------------  
		 
	private:
		void prepareFrame(std::vector<std::function<void()>> recreatefuncs);
		void submitFrame(std::vector<std::function<void()>> recreatefuncs);

		//recreate functions
		void recreate(std::vector<std::function<void()>> recreatefuncs);
	};
}