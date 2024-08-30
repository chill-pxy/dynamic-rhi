#include<stdexcept>

#include"../../Include/Vulkan/VulkanDRHI.h"

namespace DRHI
{
	VulkanDRHI::VulkanDRHI(RHICreatInfo createInfo)
	{
		_glfwWindowCreateInfo = createInfo.glfwWindowCreateInfo;

		_instance = new VkInstance();
		_surface;
		_physicalDevice;
		_device;
		_queueFamilyIndices;
		_graphicQueue;
		_presentQueue;
		_swapChain;
		_swapChainImageFormat;
		_swapChainExtent;
		_swapChainImages;
		_swapChainImageViews;
		_swapChainFramebuffers;
		_commandPool;
	}

	void VulkanDRHI::initialize()
	{
		_glfwWindow = createGlfwWindow(&_extensions, _glfwWindowCreateInfo);
		createInstance(_instance, _extensions);
		createSurface(&_surface, _instance, _glfwWindow);
		pickPhysicalDevice(&_physicalDevice, _instance, 0);
		pickGraphicQueueFamily(&_physicalDevice, (uint32_t)-1);
		createLogicalDevice(&_device, &_physicalDevice, &_graphicQueue, &_presentQueue, &_surface, &_queueFamilyIndices);
		createSwapChain(&_swapChain, &_physicalDevice, &_device, &_surface, _glfwWindow, _swapChainImages, &_swapChainImageFormat, _swapChainExtent);
		createImageViews(&_device, _swapChainImageViews, _swapChainImages, &_swapChainImageFormat);
		createCommandPool(&_commandPool, &_device, _queueFamilyIndices);
	}
}