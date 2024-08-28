#include<stdexcept>

#include"VulkanDRHI.h"

namespace DRHI
{
	void VulkanDRHI::initialize()
	{
        createInstance(&_instance, _extensions);
        createSurface(&_surface, &_instance, _window);
        pickPhysicalDevice(&_physicalDevice, &_instance, 0);
		pickGraphicQueueFamily(&_physicalDevice, (uint32_t)-1);
		createLogicalDevice(&_device, &_physicalDevice, &_graphicQueue, &_presentQueue, &_surface, &_queueFamilyIndices);
	}
}