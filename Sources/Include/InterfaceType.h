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

	typedef struct DynamicFormat
	{
		DynamicFormat(API api)
		{
			switch (api)
			{
			case DRHI::VULKAN:
				FORMAT_R32G32B32_SFLOAT = VK_FORMAT_R32G32B32_SFLOAT;
				FORMAT_R32G32_SFLOAT = VK_FORMAT_R32G32_SFLOAT;
				FORMAT_R8G8B8A8_UNORM = VK_FORMAT_R8G8B8A8_UNORM;
				FORMAT_R8G8B8A8_SRGB = VK_FORMAT_R8G8B8A8_SRGB;
				break;
			case DRHI::DIRECT3D12:
				break;
			}
		}

		uint32_t FORMAT_R32G32B32_SFLOAT{ 0 }, FORMAT_R32G32_SFLOAT{ 0 }, FORMAT_R8G8B8A8_UNORM{ 0 }, FORMAT_R8G8B8A8_SRGB{ 0 };

	}DynamicFormat;

	typedef struct DynamicImageTiling
	{
		DynamicImageTiling(API api)
		{
			switch (api)
			{
			case DRHI::VULKAN:
				IMAGE_TILING_OPTIMAL = VK_IMAGE_TILING_OPTIMAL;
				IMAGE_TILING_LINEAR = VK_IMAGE_TILING_LINEAR;
				IMAGE_TILING_DRM_FORMAT_MODIFIER_EXT = VK_IMAGE_TILING_DRM_FORMAT_MODIFIER_EXT;
				IMAGE_TILING_MAX_ENUM = VK_IMAGE_TILING_MAX_ENUM;
				break;
			case DRHI::DIRECT3D12:
				break;
			}
		}

		uint32_t IMAGE_TILING_OPTIMAL{0};
		uint32_t IMAGE_TILING_LINEAR{0};
		uint32_t IMAGE_TILING_DRM_FORMAT_MODIFIER_EXT{0};
		uint32_t IMAGE_TILING_MAX_ENUM{0};
	}DynamicImageTiling;

	typedef struct DynamicImageUsageFlagBits
	{
		DynamicImageUsageFlagBits(API api)
		{
			switch (api)
			{
			case DRHI::VULKAN:
				IMAGE_USAGE_SAMPLED_BIT = VK_IMAGE_USAGE_SAMPLED_BIT;
				IMAGE_USAGE_TRANSFER_DST_BIT = VK_IMAGE_USAGE_TRANSFER_DST_BIT;
				break;
			case DRHI::DIRECT3D12:
				break;
			}
		}

		uint32_t IMAGE_USAGE_SAMPLED_BIT{ 0 }, IMAGE_USAGE_TRANSFER_DST_BIT{ 0 };
	}DynamicImageUsageFlagBits;

	typedef struct DynamicMemoryPropertyFlags
	{
		DynamicMemoryPropertyFlags(API api)
		{
			switch (api)
			{
			case DRHI::VULKAN:
				MEMORY_PROPERTY_DEVICE_LOCAL_BIT = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
				break;
			case DRHI::DIRECT3D12:
				break;
			}
		}

		uint32_t MEMORY_PROPERTY_DEVICE_LOCAL_BIT{ 0 };
	}DynamicMemoryPropertyFlags;

	typedef struct DynamicBufferUsageFlags
	{
		DynamicBufferUsageFlags(API api)
		{
			switch (api)
			{
			case DRHI::VULKAN:
				BUFFER_USAGE_VERTEX_BUFFER_BIT = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
				BUFFER_USAGE_INDEX_BUFFER_BIT = VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
				BUFFER_USAGE_TRANSFER_DST_BIT = VK_BUFFER_USAGE_TRANSFER_DST_BIT;
				BUFFER_USAGE_TRANSFER_SRC_BIT = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
				break;
			case DRHI::DIRECT3D12:
				break;
			}
		}

		uint32_t BUFFER_USAGE_VERTEX_BUFFER_BIT{ 0 }, BUFFER_USAGE_INDEX_BUFFER_BIT{ 0 }, BUFFER_USAGE_TRANSFER_DST_BIT{ 0 }, BUFFER_USAGE_TRANSFER_SRC_BIT{ 0 };

	}; DynamicBufferUsageFlags;

	typedef struct DynamicPipelineBindPoint
	{
		DynamicPipelineBindPoint(API api)
		{
			switch (api)
			{
			case DRHI::VULKAN:
				PIPELINE_BIND_POINT_GRAPHICS = VK_PIPELINE_BIND_POINT_GRAPHICS;
				break;
			case DRHI::DIRECT3D12:
				break;
			}
		}

		uint32_t PIPELINE_BIND_POINT_GRAPHICS{ 0 };

	}DynamicPipelineBindPoint;

	typedef struct DynamicSamplerAddressMode
	{
		DynamicSamplerAddressMode(API api)
		{
			switch (api)
			{
			case DRHI::VULKAN:
				SAMPLER_ADDRESS_MODE_REPEAT = VK_SAMPLER_ADDRESS_MODE_REPEAT;
				SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT = VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT;
				SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
				SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
				SAMPLER_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE = VK_SAMPLER_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE;
				SAMPLER_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE_KHR = VK_SAMPLER_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE_KHR;
				SAMPLER_ADDRESS_MODE_MAX_ENUM = VK_SAMPLER_ADDRESS_MODE_MAX_ENUM;
				break;
			case DRHI::DIRECT3D12:
				break;
			}
		}

		uint32_t  SAMPLER_ADDRESS_MODE_REPEAT{ 0 }, SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT{ 0 },
			SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE{ 0 }, SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER{ 0 },
			SAMPLER_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE{ 0 },SAMPLER_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE_KHR{ 0 },
			SAMPLER_ADDRESS_MODE_MAX_ENUM{ 0 };

	}DynamicSamplerAddressMode;

	typedef struct DynamicBorderColor
	{
		DynamicBorderColor(API api)
		{
			switch (api)
			{
			case DRHI::VULKAN:
				BORDER_COLOR_FLOAT_TRANSPARENT_BLACK = VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK;
				BORDER_COLOR_INT_TRANSPARENT_BLACK = VK_BORDER_COLOR_INT_TRANSPARENT_BLACK;
				BORDER_COLOR_FLOAT_OPAQUE_BLACK = VK_BORDER_COLOR_FLOAT_OPAQUE_BLACK;
				BORDER_COLOR_INT_OPAQUE_BLACK = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
				BORDER_COLOR_FLOAT_OPAQUE_WHITE = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;
				BORDER_COLOR_INT_OPAQUE_WHITE = VK_BORDER_COLOR_INT_OPAQUE_WHITE;
				BORDER_COLOR_FLOAT_CUSTOM_EXT = VK_BORDER_COLOR_FLOAT_CUSTOM_EXT;
				BORDER_COLOR_INT_CUSTOM_EXT = VK_BORDER_COLOR_INT_CUSTOM_EXT;
				BORDER_COLOR_MAX_ENUM = VK_BORDER_COLOR_MAX_ENUM;
				break;
			case DRHI::DIRECT3D12:
				break;
			}
		}

		uint32_t BORDER_COLOR_FLOAT_TRANSPARENT_BLACK{ 0 },
			BORDER_COLOR_INT_TRANSPARENT_BLACK{ 0 },
			BORDER_COLOR_FLOAT_OPAQUE_BLACK{ 0 },
			BORDER_COLOR_INT_OPAQUE_BLACK{ 0 },
			BORDER_COLOR_FLOAT_OPAQUE_WHITE{ 0 },
			BORDER_COLOR_INT_OPAQUE_WHITE{ 0 },
			BORDER_COLOR_FLOAT_CUSTOM_EXT{ 0 },
			BORDER_COLOR_INT_CUSTOM_EXT{ 0 },
			BORDER_COLOR_MAX_ENUM{ 0 };

	}DynamicBorderColor;

	typedef struct DynamicExtent2D
	{
		uint32_t width;
		uint32_t height;
	}DynamicExtent2D;

	typedef struct DynamicOffset2D
	{
		int32_t x;
		int32_t y;
	}DynamicOffset2D;

	typedef struct DynamicRect2D
	{
		DynamicExtent2D extent;
		DynamicOffset2D offset;
	}DynamicRect2D;

	typedef struct DynamicSmplerCreateInfo
	{
		uint32_t sampleraAddressMode;
		uint32_t borderColor;
	}DynamicSmplerCreateInfo;

	class DynamicBuffer
	{
	public:
		std::variant<VkBuffer> internalID;

		inline VkBuffer getVulkanBuffer() { return std::get<VkBuffer>(internalID); }
	};

	class DynamicDeviceMemory
	{
	public:
		std::variant<VkDeviceMemory> internalID;

		inline VkDeviceMemory getVulkanDeviceMemory() { return std::get<VkDeviceMemory>(internalID); }
	};

	class DynamicImage
	{
	public:
		std::variant<VkImage> internalID;

		inline VkImage getVulkanImage() { return std::get<VkImage>(internalID); }
	};

	class DynamicImageView
	{
	public:
		std::variant<VkImageView> internalID;

		inline VkImageView getVulkanImageView() { return std::get<VkImageView>(internalID); }
	};

	class DynamicSampler
	{
	public:
		std::variant<VkSampler> internalID;

		inline VkSampler getVulkanSampler() { return std::get<VkSampler>(internalID); }
	};

	class DynamicPipeline
	{
	public:
		std::variant<VkPipeline> internalID;

		inline VkPipeline getVulkanPipeline() { return std::get<VkPipeline>(internalID); }
	};

	class DynamicPipelineLayout
	{
	public:
		std::variant<VkPipelineLayout> internalID;

		inline VkPipelineLayout getVulkanPipelineLayout() { return std::get<VkPipelineLayout>(internalID); }
	};

	class DynamicDescriptorSet
	{
	public:
		std::variant<VkDescriptorSet> internalID;

		inline VkDescriptorSet getVulkanDescriptorSet() { return std::get<VkDescriptorSet>(internalID); }
	};

	class DynamicDescriptorSetLayout
	{
	public:
		std::variant<VkDescriptorSetLayout> internalID;

		inline VkDescriptorSetLayout getVulkanDescriptorSetLayout() { return std::get<VkDescriptorSetLayout>(internalID); }
	};

	class DynamicDescriptorBufferInfo
	{
	public:
		std::variant<VkDescriptorBufferInfo> internalID;

		void set(API api, DynamicBuffer buffer, uint64_t bufferSize, uint32_t offset = 0)
		{
			switch (api)
			{
			case DRHI::VULKAN:
			{
				VkDescriptorBufferInfo vkinfo{};
				vkinfo.buffer = buffer.getVulkanBuffer();
				vkinfo.offset = offset;
				vkinfo.range = bufferSize;

				internalID = vkinfo;
			}break;
			case DRHI::DIRECT3D12:
				break;
			}
			
		}
		inline VkDescriptorBufferInfo getVulkanDescriptorBufferInfo() { return std::get<VkDescriptorBufferInfo>(internalID); }
	};

	class DynamicVertexInputBindingDescription
	{
	public:
		std::variant<VkVertexInputBindingDescription> internalID;

		void set(API api, uint32_t binding, uint32_t stride)
		{
			switch (api)
			{
			case DRHI::VULKAN:
			{
				VkVertexInputBindingDescription vkinfo{};
				vkinfo.binding = binding;
				vkinfo.stride = stride;
				vkinfo.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

				internalID = vkinfo;
					
			}break;
			case DRHI::DIRECT3D12:
				break;
			}
		}

		inline VkVertexInputBindingDescription getVulkanVertexInputBindingDescription(){ return std::get<VkVertexInputBindingDescription>(internalID); }
	};

	class DynamicVertexInputAttributeDescription
	{
	public:
		std::variant<VkVertexInputAttributeDescription> internalID;
		
		void set(API api, uint32_t location, uint32_t binding, uint32_t format, uint32_t offset)
		{
			switch (api)
			{
			case DRHI::VULKAN:
			{
				VkVertexInputAttributeDescription vkinfo{};
				vkinfo.binding = binding;
				vkinfo.location = location;
				vkinfo.offset = offset;
				vkinfo.format = (VkFormat)format;

				internalID = vkinfo;
			}break;
			case DRHI::DIRECT3D12:
				break;
			}
		}

		inline VkVertexInputAttributeDescription getVulkanVertexInputAttributeDescription() { return std::get<VkVertexInputAttributeDescription>(internalID); }
	};


}




