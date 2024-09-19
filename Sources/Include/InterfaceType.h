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

	class DynamicImage
	{
	public:
		std::variant<VkImage> internalID;

		VkImage getVulkanImage() { return std::get<VkImage>(internalID); }
	};

	class DynamicImageView
	{
	public:
		std::variant<VkImageView> internalID;

		VkImageView getVulkanImageView() { return std::get<VkImageView>(internalID); }
	};

	class DynamicSampler
	{
	public:
		std::variant<VkSampler> internalID;

		VkSampler getVulkanSampler() { return std::get<VkSampler>(internalID); }
	};

	class DynamicDescriptorBufferInfo
	{
	public:
		std::variant<VkDescriptorBufferInfo> internalID;

		void set(DynamicBuffer buffer, uint64_t bufferSize, uint32_t offset = 0)
		{
			VkDescriptorBufferInfo vkinfo{};
			vkinfo.buffer = buffer.getVulkanBuffer();
			vkinfo.offset = offset;
			vkinfo.range = bufferSize;

			internalID = vkinfo;
		}
		VkDescriptorBufferInfo getVulkanDrscriptorBufferInfo() { return std::get<VkDescriptorBufferInfo>(internalID); }
	};
}




