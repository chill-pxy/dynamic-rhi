#pragma once

#include<stdexcept>

#include"../../Include/InterfaceType.h"
#include"VulkanQueueFamily.h"

namespace drhi
{
    void createLogicalDevice(VkDevice* device, VkPhysicalDevice* physicalDevice, VkQueue* graphicsQueue, VkQueue* presentQueue, VkSurfaceKHR* surface, QueueFamilyIndices* queueFamilyIndices, bool supportRayTracing);
}