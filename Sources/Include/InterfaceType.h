#pragma once

#define VK_USE_PLATFORM_WIN32_KHR
#include<volk.h>

#define NOMINMAX
#include<Windows.h>

#include<variant>

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

const int MAX_FRAMES_IN_FLIGHT = 3;

namespace DRHI
{
	typedef enum API
	{
		VULKAN,
		DIRECT3D12
	}API;

	class DynamicBuffer
	{
	public:
		std::variant<VkBuffer> internalID;

		VkBuffer getVulkanBuffer() { return std::get<VkBuffer>(internalID); }
	};

	class DynamicDeviceMemory
	{
	public:
		std::variant<VkDeviceMemory> internalID;

		VkDeviceMemory getVulkanDeviceMemory() { return std::get<VkDeviceMemory>(internalID); }
	};
}




