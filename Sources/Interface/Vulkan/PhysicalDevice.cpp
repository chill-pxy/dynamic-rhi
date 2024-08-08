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

        VkPhysicalDevice* vdevice = new VkPhysicalDevice();
        *vdevice = devices[id];
        _runtimePhysicalDevice = vdevice;


        if (vdevice == VK_NULL_HANDLE) {
            throw std::runtime_error("failed to find a suitable GPU!");
        }

        
        
	}

    void PhysicalDevice::pickGraphicQueueFamily()
    {
        uint32_t count;

        VkPhysicalDevice* device = getVkPhysicalDevice();

        vkGetPhysicalDeviceQueueFamilyProperties(*device, &count, NULL);
        VkQueueFamilyProperties* queues = (VkQueueFamilyProperties*)malloc(sizeof(VkQueueFamilyProperties) * count);
        vkGetPhysicalDeviceQueueFamilyProperties(*device, &count, queues);
        for (uint32_t i = 0; i < count; i++)
        {
            if (queues[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
            {
                _queueFamily = i;
                break;
            }
        }
        free(queues);
        if (!(_queueFamily != (uint32_t)-1))
        {
            throw std::runtime_error("error nums of queue family!");
        }
    }
}

#endif