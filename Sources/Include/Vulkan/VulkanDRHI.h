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
#include "VulkanGlfwWindow.h"
#include "VulkanDescriptor.h"
#include "VulkanGraphicsPipeline.h"

namespace DRHI
{
	struct RHICreatInfo
	{
		VulkanGlfwWindowCreateInfo glfwWindowCreateInfo;
	};

	class VulkanDRHI : public DynamicRHI
	{
	public:
		GLFWwindow*              _glfwWindow{ VK_NULL_HANDLE };
		std::vector<const char*> _extensions;
		
	private:
		VulkanGlfwWindowCreateInfo   _glfwWindowCreateInfo{ VK_NULL_HANDLE };
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
		VkPipeline                   _graphicsPipeline{ VK_NULL_HANDLE };
		VkPipelineLayout             _pipelineLayout{ VK_NULL_HANDLE };

	public:
		VulkanDRHI() = delete;
		VulkanDRHI(RHICreatInfo createInfo);
		
		virtual void initialize();
		virtual void clean();

		void createPipeline(VulkanPipelineCreateInfo info);
	};
}