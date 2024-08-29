#pragma once

#include<vector>

#include "../DynamicRHI.h"
#include "VulkanInstance.h"
#include "VulkanSurface.h"
#include "VulkanPhysicalDevice.h"
#include "VulkanDevice.h"
#include "VulkanQueueFamily.h"
#include "VulkanSwapChain.h"
#include "VulkanCommanPool.h"
#include "VulkanGlfwWindow.h"

namespace DRHI
{
	struct RHICreatInfo
	{
		VulkanGlfwWindowCreateInfo glfwWindowCreateInfo;
	};

	class VulkanDRHI : public DynamicRHI
	{
	private:
		VulkanGlfwWindow _glfwWindow;
		VulkanGlfwWindowCreateInfo _glfwWindowCreateInfo;

		VkInstance _instance;

		VkSurfaceKHR _surface;

		VkPhysicalDevice _physicalDevice;

		VkDevice _device;
		QueueFamilyIndices _queueFamilyIndices;

		VkQueue _graphicQueue;
		VkQueue _presentQueue;

		VkSwapchainKHR             _swapChain;
		VkFormat                   _swapChainImageFormat;
		VkExtent2D                 _swapChainExtent;
		std::vector<VkImage>       _swapChainImages;
		std::vector<VkImageView>   _swapChainImageViews;
		std::vector<VkFramebuffer> _swapChainFramebuffers;

		VkCommandPool _commandPool;

	public:
		VulkanDRHI() = delete;
		VulkanDRHI(RHICreatInfo createInfo);

		void initialize()
		{
			_glfwWindow.initialize(_glfwWindowCreateInfo);
			createInstance(&_instance, _glfwWindow.getVulkanGlfwWindowExtensions());
			createSurface(&_surface, &_instance, _glfwWindow.getVulkanGlfwWindow());
			pickPhysicalDevice(&_physicalDevice, &_instance, 0);
			pickGraphicQueueFamily(&_physicalDevice, (uint32_t)-1);
			createLogicalDevice(&_device, &_physicalDevice, &_graphicQueue, &_presentQueue, &_surface, &_queueFamilyIndices);
			createSwapChain(&_swapChain, &_physicalDevice, &_device, &_surface, _glfwWindow.getVulkanGlfwWindow(), _swapChainImages, &_swapChainImageFormat, _swapChainExtent);
			createImageViews(&_device, _swapChainImageViews, _swapChainImages, &_swapChainImageFormat);
			createCommandPool(&_commandPool, &_device, _queueFamilyIndices);
		}
	};
}