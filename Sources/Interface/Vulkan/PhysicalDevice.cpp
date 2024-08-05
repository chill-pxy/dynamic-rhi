#include "../../Include/InterfaceConfig.h"

#ifdef VULKAN_IMPLEMENTATION

#include <stdexcept>
#include <vector>

#include "../../Include/PhysicalDevice.h"


namespace DRHI
{
	void PhysicalDevice::pickPhysicalDevice(unsigned int id, Instance* instance)
	{
        VkInstance* vinstance = instance->getVkInstance();
        uint32_t deviceCount = 0;
        vkEnumeratePhysicalDevices(*vinstance, &deviceCount, nullptr);

        if (deviceCount == 0) {
            throw std::runtime_error("failed to find GPUs with Vulkan support!");
        }

        std::vector<VkPhysicalDevice> devices(deviceCount);
        vkEnumeratePhysicalDevices(*vinstance, &deviceCount, devices.data());

        VkPhysicalDevice* device = &devices[id];
        this->_runtimePhysicalDevice = device;

        if (device == VK_NULL_HANDLE) {
            throw std::runtime_error("failed to find a suitable GPU!");
        }
	}
}

#endif