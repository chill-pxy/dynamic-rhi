#pragma once

#include"../../Include/InterfaceType.h"
#include"VulkanQueueFamily.h"

namespace DRHI
{
    void createLogicalDevice(VkDevice* device, VkPhysicalDevice* physicalDevice, VkQueue* graphicsQueue, VkQueue* presentQueue, VkSurfaceKHR* surface, QueueFamilyIndices* queueFamilyIndices);
}