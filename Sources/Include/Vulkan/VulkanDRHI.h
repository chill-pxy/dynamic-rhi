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
		std::vector<VkCommandBuffer> _commandBuffers;
		VkCommandPool                _commandPool{ VK_NULL_HANDLE };
		VkPipelineCache              _pipelineCache{ VK_NULL_HANDLE };
		PlatformInfo                 _platformInfo{};
		Semaphores                   _semaphores{ VK_NULL_HANDLE, VK_NULL_HANDLE };
		VkPipelineStageFlags         _submitPipelineStages{ VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
		uint32_t                     _viewPortWidth{ 0 };
		uint32_t                     _viewPortHeight{ 0 };
		std::vector<VkFence>         _waitFences;

		bool _prepare{ false };

		// Active frame buffer index
		uint32_t _currentBuffer = 0;
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
		virtual void frameOnTick(std::function<void()> recreatefunc);

		//draw function
		virtual void drawIndexed(uint32_t index, uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, uint32_t vertexOffset, uint32_t firstInstance);

		//scissor
		virtual void setScissor(uint32_t index, uint32_t firstScissor, uint32_t scissorCount, DynamicRect2D rect);

		//clear functions
		virtual void clean();

		//command functions
		virtual void beginCommandBuffer(uint32_t index);
		virtual void endCommandBuffer(uint32_t index);
		virtual uint32_t getCommandBufferSize();

		//buffer functions
		virtual void bindVertexBuffers(DynamicBuffer* vertexBuffer, uint32_t index);
		virtual void bindIndexBuffer(DynamicBuffer* indexBuffer, uint32_t index, uint32_t indexType);
		virtual uint32_t getCurrentBuffer();
		virtual void createDynamicBuffer(DynamicBuffer* buffer, DynamicDeviceMemory* deviceMemory, uint64_t bufferSize, void* bufferData, uint32_t usage, uint32_t memoryProperty);
		virtual void createUniformBuffer(std::vector<DynamicBuffer>* uniformBuffers, std::vector<DynamicDeviceMemory>* uniformBuffersMemory, std::vector<void*>* uniformBuffersMapped, uint32_t bufferSize);
		//virtual void createDynamicBuffer(DynamicBuffer* buffer, DynamicDeviceMemory* memory, uint64_t bufferSize, uint32_t usage);
		virtual void clearBuffer(DynamicBuffer* buffer, DynamicDeviceMemory* memory);
		virtual void flushBuffer(DynamicDeviceMemory* memory, uint32_t size, uint32_t offset);	
		virtual void flushBuffer(DynamicDeviceMemory* memory, uint32_t offset);

		//memory functions
		virtual void mapMemory(DynamicDeviceMemory* memory, uint32_t offset, uint32_t size, void* data);
		virtual void unmapMemory(DynamicDeviceMemory* memory);

		//descriptor funcions
		virtual void createDescriptorPool(DynamicDescriptorPool* descriptorPool);
		virtual void createDescriptorSet(DynamicDescriptorSet* descriptorSet, DynamicDescriptorSetLayout* descriptorSetLayout, DynamicDescriptorPool* descriptorPool, std::vector<DynamicWriteDescriptorSet>* wds);
		virtual void bindDescriptorSets(DynamicDescriptorSet* descriptorSet, DynamicPipelineLayout pipelineLayout, uint32_t bindPoint, uint32_t index);
		virtual void createDescriptorSetLayout(DynamicDescriptorSetLayout* descriptorSetLayout, std::vector<DynamicDescriptorSetLayoutBinding>* dsbs);

		//texture funcitons
		virtual void createTextureImage(DynamicImage* textureImage, DynamicDeviceMemory* textureMemory, int texWidth, int texHeight, int texChannels, stbi_uc* pixels);
		virtual void createTextureSampler(DynamicSampler* textureSampler);

		//image functions
		virtual void createImageView(DynamicImageView* imageView, DynamicImage* image, uint32_t imageFormat);
		virtual void createImage(DynamicImage* image, uint32_t width, uint32_t height,
			uint32_t format, uint32_t imageTiling, uint32_t imageUsageFlagBits, uint32_t memoryPropertyFlags, DynamicDeviceMemory* imageMemory);
		virtual void copyBufferToImage(DynamicBuffer* buffer, DynamicImage* image, uint32_t width, uint32_t height);
		virtual void createSampler(DynamicSampler* sampler, DynamicSmplerCreateInfo createInfo);
		virtual void clearImage(DynamicSampler* sampler, DynamicImageView* imageView, DynamicImage* image, DynamicDeviceMemory* memory);

		//pipeline functions
		virtual void createPipelineLayout(DynamicPipelineLayout* pipelineLayout, DynamicPipelineLayoutCreateInfo* createInfo);
		virtual void createPipeline(DynamicPipeline* pipeline, DynamicPipelineLayout* pipelineLayout, DynamicPipelineCreateInfo info);
		virtual void bindPipeline(DynamicPipeline pipeline, uint32_t bindPoint, uint32_t index);
		        VkPipelineRenderingCreateInfoKHR getPipelineRenderingCreateInfo();

		//cmd functions
		virtual void cmdPushConstants(uint32_t index, DynamicPipelineLayout* layout, uint32_t stage, uint32_t offset, uint32_t size, void* value);

		//-------------------------------------------------------------------------------------------------------------------------- 
		//--------------------------------------------------------------------------------------------------------------------------  
		//--------------------------------------------------------------------------------------------------------------------------
		//-------------------------------------------------------------------------------------------------------------------------- 
		//--------------------------------------------------------------------------------------------------------------------------  
		 
	private:
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

		void prepareFrame(std::function<void()> recreatefunc);
		void submitFrame(std::function<void()> recreatefunc);

		//recreate functions
		void recreate(std::function<void()> recreatefunc);
	};
}