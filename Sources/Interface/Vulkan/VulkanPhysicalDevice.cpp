#include<vector>
#include<stdexcept>

#include"../../Include/Vulkan/VulkanPhysicalDevice.h"

namespace DRHI
{
    void pickPhysicalDevice(VkPhysicalDevice* physicalDevice, VkInstance* instance, unsigned int id)
    {
        uint32_t deviceCount = 0;
        vkEnumeratePhysicalDevices(*instance, &deviceCount, nullptr);

        if (deviceCount == 0) {
            throw std::runtime_error("failed to find GPUs with Vulkan support!");
        }

        std::vector<VkPhysicalDevice> devices(deviceCount);
        vkEnumeratePhysicalDevices(*instance, &deviceCount, devices.data());

        VkPhysicalDevice* vdevice = new VkPhysicalDevice();
        *vdevice = devices[id];
        physicalDevice = vdevice;

        if (vdevice == VK_NULL_HANDLE) {
            throw std::runtime_error("failed to find a suitable GPU!");
        }
    }

    void pickGraphicQueueFamily(VkPhysicalDevice* physicalDevice, uint32_t queueFamily)
    {
        uint32_t count;

        vkGetPhysicalDeviceQueueFamilyProperties(*physicalDevice, &count, NULL);
        VkQueueFamilyProperties* queues = (VkQueueFamilyProperties*)malloc(sizeof(VkQueueFamilyProperties) * count);
        vkGetPhysicalDeviceQueueFamilyProperties(*physicalDevice, &count, queues);
        for (uint32_t i = 0; i < count; i++)
        {
            if (queues[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
            {
                queueFamily = i;
                break;
            }
        }
        free(queues);
        if (!(queueFamily != (uint32_t)-1))
        {
            throw std::runtime_error("error nums of queue family!");
        }
    }
}