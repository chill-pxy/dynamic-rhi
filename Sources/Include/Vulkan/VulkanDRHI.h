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

		virtual void initialize();
		inline VulkanGlfwWindow getVulkanGlfwWindow() { return _glfwWindow; }
	};
}