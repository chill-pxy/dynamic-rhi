#include "../../Include/InterfaceConfig.h"

#ifdef VULKAN_IMPLEMENTATION

#include "../../Include/Device.h"

namespace DRHI
{
	void Device::createLogicalDevice(PhysicalDevice* phyDevice, CommandQueue* graphicQueue)
	{
        int device_extension_count = 1;
        const char* device_extensions[] = { "VK_KHR_swapchain" };
        const float queue_priority[] = { 1.0f };
        VkDevice* device = new VkDevice();
        
        const char* ext_name[] =
        {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME
        };

        VkDeviceQueueCreateInfo queue_info[1] = {};
        queue_info[0].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queue_info[0].queueFamilyIndex = phyDevice->getQueueFamily();
        queue_info[0].queueCount = 1;
        queue_info[0].pQueuePriorities = queue_priority;

        VkDeviceCreateInfo create_info = {};
        create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        create_info.queueCreateInfoCount = sizeof(queue_info) / sizeof(queue_info[0]);
        create_info.pQueueCreateInfos = queue_info;
        create_info.enabledExtensionCount = device_extension_count;
        create_info.ppEnabledExtensionNames = device_extensions;

        VkResult res = vkCreateDevice(*(phyDevice->getVkPhysicalDevice()), &create_info, nullptr, device);
        if (res != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create device!");
        }
        vkGetDeviceQueue(*device, phyDevice->getQueueFamily(), 0, graphicQueue->getVkQueue());
        _runtimeDevice = device;
	}
}

#endif
