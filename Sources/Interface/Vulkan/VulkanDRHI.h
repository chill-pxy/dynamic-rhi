#pragma once

#include<vector>

#include "../../Include/DynamicRHI.h"
#include "VulkanInstance.h"
#include "VulkanSurface.h"
#include "VulkanPhysicalDevice.h"
#include "VulkanDevice.h"
#include "VulkanQueueFamily.h"

namespace DRHI
{
	class VulkanDRHI : public DynamicRHI
	{
	private:
		std::vector<const char*> _extensions;
		GLFWwindow* _window;

		VkInstance _instance;

		VkSurfaceKHR _surface;

		VkPhysicalDevice _physicalDevice;

		VkDevice _device;
		QueueFamilyIndices _queueFamilyIndices;

		VkQueue _graphicQueue;
		VkQueue _presentQueue;

	public:
		virtual void initialize() final override;
	};
}