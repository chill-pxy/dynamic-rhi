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

namespace DRHI
{
	struct RHICreateInfo
	{
		PlatformInfo platformInfo;
	};

	struct PipelineCreateInfo
	{
		const char* vertexShader;
		const char* fragmentShader;
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
		VkFormat                     _swapChainImageFormat{ VK_FORMAT_UNDEFINED };
		VkExtent2D                   _swapChainExtent{ 0 };
		std::vector<VkImage>         _swapChainImages;
		std::vector<VkImageView>     _swapChainImageViews;
		std::vector<VkFramebuffer>   _swapChainFramebuffers;
		std::vector<VkCommandBuffer> _commandBuffers;
		VkCommandPool                _commandPool{ VK_NULL_HANDLE };
		VkDescriptorSetLayout        _descriptorSetLayout{ VK_NULL_HANDLE };
		VkDescriptorPool             _descriptorPool{ VK_NULL_HANDLE };
		VkDescriptorSet              _descriptorSet{ VK_NULL_HANDLE };
		VkPipeline                   _graphicsPipeline{ VK_NULL_HANDLE };
		VkPipelineLayout             _pipelineLayout{ VK_NULL_HANDLE };
		PlatformInfo                 _platformInfo{};
		Semaphores                   _semaphores{ VK_NULL_HANDLE, VK_NULL_HANDLE };
		uint32_t                     _viewPortWidth{ 0 };
		uint32_t                     _viewPortHeight{ 0 };

		// Active frame buffer index
		uint32_t _currentBuffer = 0;
		// Contains command buffers and semaphores to be presented to the queue
		VkSubmitInfo _submitInfo;

	public:
		VulkanDRHI() = delete;
		VulkanDRHI(RHICreateInfo createInfo);
		
		//initialize vulkan rhi member
		virtual void initialize();
		//clean vulkan rhi member
		virtual void clean();
		//call vkCommandBegin function
		virtual void prepareCommandBuffer();
		//call within render loop
		virtual void frameOnTick();
		
		
		
		//Buffer class
		virtual void iCreateDynamicBuffer(DynamicBuffer* vertexBuffer, DynamicDeviceMemory* deviceMemory, uint64_t bufferSize, void* bufferData);
		//
		virtual void iBindVertexBuffer();
		//
		virtual void iBindIndexBuffer();
		
		
		
		//create the particular pipeline
		void createPipeline(PipelineCreateInfo info);
		 
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