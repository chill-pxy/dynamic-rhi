#include<set>
#include<stdexcept>

#include"../../Include/Vulkan/VulkanDevice.h"

const std::vector<const char*> deviceExtensions =
{
    VK_KHR_DYNAMIC_RENDERING_EXTENSION_NAME,
    VK_KHR_SWAPCHAIN_EXTENSION_NAME,
   
};

const std::vector<const char*> validationLayers = 
{
    "VK_LAYER_KHRONOS_validation"
};

namespace DRHI
{
    void createLogicalDevice(VkDevice* device, VkPhysicalDevice* physicalDevice, VkQueue* graphicsQueue, VkQueue* presentQueue, VkSurfaceKHR* surface, QueueFamilyIndices* queueFamilyIndices)
    {
        QueueFamilyIndices indices = findQueueFamilies(*physicalDevice, *surface);

        std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
        std::set<uint32_t> uniqueQueueFamilies = { indices.graphicsFamily.value(), indices.presentFamily.value() };

        float queuePriority = 1.0f;
        for (uint32_t queueFamily : uniqueQueueFamilies) 
        {
            VkDeviceQueueCreateInfo queueCreateInfo{};
            queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueCreateInfo.queueFamilyIndex = queueFamily;
            queueCreateInfo.queueCount = 1;
            queueCreateInfo.pQueuePriorities = &queuePriority;
            queueCreateInfos.push_back(queueCreateInfo);
        }

        VkPhysicalDeviceFeatures deviceFeatures{};
        deviceFeatures.samplerAnisotropy = VK_TRUE;
        deviceFeatures.geometryShader = VK_TRUE;
        deviceFeatures.tessellationShader = VK_TRUE;

        constexpr VkPhysicalDeviceDynamicRenderingFeaturesKHR dynamicRenderingFeature
        {
            .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DYNAMIC_RENDERING_FEATURES_KHR,
            .dynamicRendering = VK_TRUE,
        };

        VkDeviceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        createInfo.pNext = &dynamicRenderingFeature;
        createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
        createInfo.pQueueCreateInfos = queueCreateInfos.data();

        createInfo.pEnabledFeatures = &deviceFeatures;

        createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
        createInfo.ppEnabledExtensionNames = deviceExtensions.data();

        if (enableValidationLayers) {
            createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
            createInfo.ppEnabledLayerNames = validationLayers.data();
        }
        else {
            createInfo.enabledLayerCount = 0;
        }

        if (vkCreateDevice(*physicalDevice, &createInfo, nullptr, device) != VK_SUCCESS) {
            throw std::runtime_error("failed to create logical device!");
        }

        vkGetDeviceQueue(*device, indices.graphicsFamily.value(), 0, graphicsQueue);
        vkGetDeviceQueue(*device, indices.presentFamily.value(), 0, presentQueue);

        *queueFamilyIndices = indices;
    }
}