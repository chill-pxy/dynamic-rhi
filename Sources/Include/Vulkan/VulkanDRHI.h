#pragma once

#include<vector>

#include "../DynamicRHI.h"
#include "VulkanInstance.h"
#include "VulkanSurface.h"
#include "VulkanPhysicalDevice.h"
#include "VulkanDevice.h"
#include "VulkanQueueFamily.h"
#include "VulkanSwapChain.h"
#include "VulkanCommand.h"
#include "VulkanDescriptor.h"
#include "VulkanGraphicsPipeline.h"
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
	private:
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
		VkDescriptorSetLayout        _descriptorSetLayout{ VK_NULL_HANDLE };
		VkDescriptorPool             _descriptorPool{ VK_NULL_HANDLE };
		VkDescriptorSet              _descriptorSet{ VK_NULL_HANDLE };
		std::vector<VkDescriptorSet> _descriptorSets;
		VkPipeline                   _graphicsPipeline{ VK_NULL_HANDLE };
		VkPipelineLayout             _pipelineLayout{ VK_NULL_HANDLE };
		VkPipelineCache              _pipelineCache{ VK_NULL_HANDLE };
		PlatformInfo                 _platformInfo{};
		Semaphores                   _semaphores{ VK_NULL_HANDLE, VK_NULL_HANDLE };
		VkPipelineStageFlags         _submitPipelineStages{ VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
		uint32_t                     _viewPortWidth{ 0 };
		uint32_t                     _viewPortHeight{ 0 };
		std::vector<VkFence>         _waitFences;

		// Active frame buffer index
		uint32_t _currentBuffer = 0;
		// Contains command buffers and semaphores to be presented to the queue
		VkSubmitInfo _submitInfo{};

	public:
		VulkanDRHI() = delete;
		VulkanDRHI(RHICreateInfo createInfo);
		
		//initialize vulkan rhi member
		virtual void initialize();
		//clean vulkan rhi member
		virtual void clean(std::vector<DynamicBuffer>* uniformBuffers, std::vector <DynamicDeviceMemory>* uniformBuffersMemory, DynamicImageView* textureImageView, DynamicSampler* textureSampler,
			DynamicImage* textureImage, DynamicDeviceMemory* textureImageMemory, DynamicBuffer* indexBuffer, DynamicDeviceMemory* indexBufferMemory, DynamicBuffer* vertexBuffer, DynamicDeviceMemory* vertexBufferMemory);
		//call vkCommandBegin function
		virtual void prepareCommandBuffer(DynamicBuffer* vertexBuffer, DynamicBuffer* indexBuffer, uint32_t indicesSize);
		//call within render loop
		virtual void frameOnTick();
		//get commandBuffersize
		virtual uint32_t getCommandBufferSize();
		//get current buffer
		virtual uint32_t getCurrentBuffer();
		//Buffer class
		virtual void createDynamicBuffer(DynamicBuffer* vertexBuffer, DynamicDeviceMemory* deviceMemory, uint64_t bufferSize, void* bufferData, const char* type);
		//unifrom buffer
		virtual void createUniformBuffer(std::vector<DynamicBuffer>* uniformBuffers, std::vector<DynamicDeviceMemory>* uniformBuffersMemory, std::vector<void*>* uniformBuffersMapped, uint32_t bufferSize);
		//create discriptor set
		virtual void createDescriptorSet(std::vector<DynamicDescriptorBufferInfo>* descriptor, DynamicImageView textureImageView, DynamicSampler textureSampler);
		//texture
		virtual void createTextureImage(DynamicImage* textureImage, DynamicDeviceMemory* textureMemory, int texWidth, int texHeight, int texChannels, stbi_uc* pixels);
		virtual void createImageView(DynamicImageView* imageView, DynamicImage* image);
		virtual void createTextureSampler(DynamicSampler* textureSampler);
		//create the particular pipeline
		virtual void createPipeline(PipelineCreateInfo info);
		 
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
		void prepareFrame();
		void submitFrame();
	};
}