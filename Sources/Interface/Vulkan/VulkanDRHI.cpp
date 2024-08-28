#include<stdexcept>

#include"VulkanDRHI.h"

namespace DRHI
{
	VulkanDRHI::VulkanDRHI(RHICreatInfo createInfo)
	{
		_window = createInfo.window;
		_extensions = createInfo.windowExtensions;
	}

	void VulkanDRHI::initialize()
	{
        createInstance(&_instance, _extensions);
        createSurface(&_surface, &_instance, _window);
        pickPhysicalDevice(&_physicalDevice, &_instance, 0);
		pickGraphicQueueFamily(&_physicalDevice, (uint32_t)-1);
		createLogicalDevice(&_device, &_physicalDevice, &_graphicQueue, &_presentQueue, &_surface, &_queueFamilyIndices);
		createSwapChain(&_swapChain, &_physicalDevice, &_device, &_surface, _window, _swapChainImages, &_swapChainImageFormat, _swapChainExtent);
		createImageViews(&_device, _swapChainImageViews, _swapChainImages, &_swapChainImageFormat);
		createCommandPool(&_commandPool, &_device, _queueFamilyIndices);
	}
}