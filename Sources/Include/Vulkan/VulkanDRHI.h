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
#include "VulkanDescriptor.h"

namespace DRHI
{
	struct RHICreatInfo
	{
		VulkanGlfwWindowCreateInfo glfwWindowCreateInfo;
	};

	class VulkanDRHI : public DynamicRHI
	{
	public:
		GLFWwindow* _glfwWindow;
		std::vector<const char*> _extensions;
		
	private:
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

		VkDescriptorSetLayout _descriptorSetLayout;
		VkDescriptorPool _descriptorPool;

	public:
		VulkanDRHI() = delete;
		VulkanDRHI(RHICreatInfo createInfo);
		
		virtual void initialize();
		virtual void clean();
	};
}