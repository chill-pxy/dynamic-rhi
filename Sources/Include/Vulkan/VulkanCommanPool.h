#pragma once

#include<stdexcept>

#include "../../Include/InterfaceType.h"
#include "VulkanDevice.h"

namespace DRHI
{
	void createCommandPool(VkCommandPool* commandPool, VkDevice* device, QueueFamilyIndices queueFamilyIndices);
}