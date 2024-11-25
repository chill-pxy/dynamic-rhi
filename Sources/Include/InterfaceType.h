#pragma once

#define VK_USE_PLATFORM_WIN32_KHR
#include<volk.h>

#define NOMINMAX
#include<Windows.h>

#include<variant>
#include<vector>

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

	class DynamicCommandBuffer
	{
	public:
		std::variant<VkCommandBuffer> internalID;

		inline VkCommandBuffer getVulkanCommandBuffer() { return std::get<VkCommandBuffer>(internalID); }

		inline bool vaild() const { return(std::holds_alternative<VkCommandBuffer>(internalID)); }
	};

	class DynamicCommandPool
	{
	public:
		std::variant<VkCommandPool> internalID;

		inline VkCommandPool getVulkanCommandPool() { return std::get<VkCommandPool>(internalID); }

		inline bool vaild() const { return(std::holds_alternative<VkCommandPool>(internalID)); }
	};

	class DynamicBuffer
	{
	public:
		std::variant<VkBuffer> internalID;

		inline VkBuffer getVulkanBuffer() { return std::get<VkBuffer>(internalID); }

		inline bool vaild() const { return(std::holds_alternative<VkBuffer>(internalID)); }
	};

	class DynamicDeviceMemory
	{
	public:
		std::variant<VkDeviceMemory> internalID;

		inline VkDeviceMemory getVulkanDeviceMemory() { return std::get<VkDeviceMemory>(internalID); }

		inline bool vaild() const { return(std::holds_alternative<VkDeviceMemory>(internalID)); }
	};

	class DynamicImage
	{
	public:
		std::variant<VkImage> internalID;

		inline VkImage getVulkanImage() { return std::get<VkImage>(internalID); }

		inline bool vaild() const { return(std::holds_alternative<VkImage>(internalID)); }
	};

	class DynamicImageView
	{
	public:
		std::variant<VkImageView> internalID;

		inline VkImageView getVulkanImageView() { return std::get<VkImageView>(internalID); }

		inline bool vaild() const { return(std::holds_alternative<VkImageView>(internalID)); }
	};

	class DynamicSampler
	{
	public:
		std::variant<VkSampler> internalID;

		inline VkSampler getVulkanSampler() { return std::get<VkSampler>(internalID); }

		inline bool vaild() const { return(std::holds_alternative<VkSampler>(internalID)); }
	};

	class DynamicPipeline
	{
	public:
		std::variant<VkPipeline> internalID;

		inline VkPipeline getVulkanPipeline() { return std::get<VkPipeline>(internalID); }

		inline bool vaild() const { return(std::holds_alternative<VkPipeline>(internalID)); }
	};

	class DynamicPipelineLayout
	{
	public:
		std::variant<VkPipelineLayout> internalID;

		inline VkPipelineLayout getVulkanPipelineLayout() { return std::get<VkPipelineLayout>(internalID); }

		inline bool vaild() const { return(std::holds_alternative<VkPipelineLayout>(internalID)); }
	};

	class DynamicDescriptorPool
	{
	public:
		std::variant<VkDescriptorPool> internalID;

		inline VkDescriptorPool getVulkanDescriptorPool() { return std::get<VkDescriptorPool>(internalID); }

		inline bool vaild() const { return(std::holds_alternative<VkDescriptorPool>(internalID)); }
	};

	class DynamicDescriptorSet
	{
	public:
		std::variant<VkDescriptorSet> internalID;

		inline VkDescriptorSet getVulkanDescriptorSet() { return std::get<VkDescriptorSet>(internalID); }

		inline bool vaild() const { return(std::holds_alternative<VkDescriptorSet>(internalID)); }
	};

	class DynamicDescriptorSetLayout
	{
	public:
		std::variant<VkDescriptorSetLayout> internalID;

		inline VkDescriptorSetLayout getVulkanDescriptorSetLayout() { return std::get<VkDescriptorSetLayout>(internalID); }

		inline bool vaild() const { return(std::holds_alternative<VkDescriptorSetLayout>(internalID)); }
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

		inline bool vaild() const { return(std::holds_alternative<VkDescriptorBufferInfo>(internalID)); }
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

		inline VkVertexInputBindingDescription getVulkanVertexInputBindingDescription() { return std::get<VkVertexInputBindingDescription>(internalID); }
	
		inline bool vaild() const { return(std::holds_alternative<VkVertexInputBindingDescription>(internalID)); }
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
	
		inline bool vaild() const { return(std::holds_alternative<VkVertexInputAttributeDescription>(internalID)); }
	};

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

	typedef struct DynamicImageAspectFlagBits
	{
		DynamicImageAspectFlagBits(API api)
		{
			switch (api)
			{
			case DRHI::VULKAN:
				IMAGE_ASPECT_COLOR_BIT = VK_IMAGE_ASPECT_COLOR_BIT;
				IMAGE_ASPECT_DEPTH_BIT = VK_IMAGE_ASPECT_DEPTH_BIT;
				IMAGE_ASPECT_STENCIL_BIT = VK_IMAGE_ASPECT_STENCIL_BIT;
				IMAGE_ASPECT_METADATA_BIT = VK_IMAGE_ASPECT_METADATA_BIT;
				IMAGE_ASPECT_PLANE_0_BIT = VK_IMAGE_ASPECT_PLANE_0_BIT;
				IMAGE_ASPECT_PLANE_1_BIT = VK_IMAGE_ASPECT_PLANE_1_BIT;
				IMAGE_ASPECT_PLANE_2_BIT = VK_IMAGE_ASPECT_PLANE_2_BIT;
				IMAGE_ASPECT_NONE = VK_IMAGE_ASPECT_NONE;
				IMAGE_ASPECT_MEMORY_PLANE_0_BIT_EXT = VK_IMAGE_ASPECT_MEMORY_PLANE_0_BIT_EXT;
				IMAGE_ASPECT_MEMORY_PLANE_1_BIT_EXT = VK_IMAGE_ASPECT_MEMORY_PLANE_1_BIT_EXT;
				IMAGE_ASPECT_MEMORY_PLANE_2_BIT_EXT = VK_IMAGE_ASPECT_MEMORY_PLANE_2_BIT_EXT;
				IMAGE_ASPECT_MEMORY_PLANE_3_BIT_EXT = VK_IMAGE_ASPECT_MEMORY_PLANE_3_BIT_EXT;
				IMAGE_ASPECT_PLANE_0_BIT_KHR = VK_IMAGE_ASPECT_PLANE_0_BIT_KHR;
				IMAGE_ASPECT_PLANE_1_BIT_KHR = VK_IMAGE_ASPECT_PLANE_1_BIT_KHR;
				IMAGE_ASPECT_PLANE_2_BIT_KHR = VK_IMAGE_ASPECT_PLANE_2_BIT_KHR;
				IMAGE_ASPECT_NONE_KHR = VK_IMAGE_ASPECT_NONE_KHR;
				IMAGE_ASPECT_FLAG_BITS_MAX_ENUM = VK_IMAGE_ASPECT_FLAG_BITS_MAX_ENUM;
				break;

			case DRHI::DIRECT3D12:
				break;
			}
		}

		uint32_t
			IMAGE_ASPECT_COLOR_BIT{ 0 },
			IMAGE_ASPECT_DEPTH_BIT{ 0 },
			IMAGE_ASPECT_STENCIL_BIT{ 0 },
			IMAGE_ASPECT_METADATA_BIT{ 0 },
			IMAGE_ASPECT_PLANE_0_BIT{ 0 },
			IMAGE_ASPECT_PLANE_1_BIT{ 0 },
			IMAGE_ASPECT_PLANE_2_BIT{ 0 },
			IMAGE_ASPECT_NONE{ 0 },
			IMAGE_ASPECT_MEMORY_PLANE_0_BIT_EXT{ 0 },
			IMAGE_ASPECT_MEMORY_PLANE_1_BIT_EXT{ 0 },
			IMAGE_ASPECT_MEMORY_PLANE_2_BIT_EXT{ 0 },
			IMAGE_ASPECT_MEMORY_PLANE_3_BIT_EXT{ 0 },
			IMAGE_ASPECT_PLANE_0_BIT_KHR{ 0 },
			IMAGE_ASPECT_PLANE_1_BIT_KHR{ 0 },
			IMAGE_ASPECT_PLANE_2_BIT_KHR{ 0 },
			IMAGE_ASPECT_NONE_KHR{ 0 },
			IMAGE_ASPECT_FLAG_BITS_MAX_ENUM{ 0 };
	};

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

	}DynamicBufferUsageFlags;

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

	typedef struct DynamicIndexType
	{
		DynamicIndexType(API api)
		{
			switch (api)
			{
			case VULKAN:
				INDEX_TYPE_UINT16 = VK_INDEX_TYPE_UINT16;
				INDEX_TYPE_UINT32 = VK_INDEX_TYPE_UINT32;
				INDEX_TYPE_NONE_KHR = VK_INDEX_TYPE_NONE_KHR;
				INDEX_TYPE_UINT8_EXT = VK_INDEX_TYPE_UINT8_EXT;
				INDEX_TYPE_NONE_NV = VK_INDEX_TYPE_NONE_NV;
				INDEX_TYPE_MAX_ENUM = VK_INDEX_TYPE_MAX_ENUM;
				break;
			case DIRECT3D12:
				break;
			}
		}

		uint32_t
			INDEX_TYPE_UINT16{ 0 },
			INDEX_TYPE_UINT32{ 0 },
			INDEX_TYPE_NONE_KHR{ 0 },
			INDEX_TYPE_UINT8_EXT{ 0 },
			INDEX_TYPE_NONE_NV{ 0 },
			INDEX_TYPE_MAX_ENUM{ 0 };

	}DynamicIndexType;

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

	typedef struct DynamicDescriptorType
	{
		DynamicDescriptorType(API api)
		{
			switch (api)
			{
			case VULKAN:
				DESCRIPTOR_TYPE_SAMPLER = VK_DESCRIPTOR_TYPE_SAMPLER;
				DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
				DESCRIPTOR_TYPE_SAMPLED_IMAGE = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
				DESCRIPTOR_TYPE_STORAGE_IMAGE = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
				DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER = VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER;
				DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER = VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER;
				DESCRIPTOR_TYPE_UNIFORM_BUFFER = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
				DESCRIPTOR_TYPE_STORAGE_BUFFER = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
				DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
				DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC;
				DESCRIPTOR_TYPE_INPUT_ATTACHMENT = VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT;
				DESCRIPTOR_TYPE_INLINE_UNIFORM_BLOCK = VK_DESCRIPTOR_TYPE_INLINE_UNIFORM_BLOCK;
				DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_KHR = VK_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_KHR;
				DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_NV = VK_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_NV;
				DESCRIPTOR_TYPE_SAMPLE_WEIGHT_IMAGE_QCOM = VK_DESCRIPTOR_TYPE_SAMPLE_WEIGHT_IMAGE_QCOM;
				DESCRIPTOR_TYPE_BLOCK_MATCH_IMAGE_QCOM = VK_DESCRIPTOR_TYPE_BLOCK_MATCH_IMAGE_QCOM;
				DESCRIPTOR_TYPE_MUTABLE_EXT = VK_DESCRIPTOR_TYPE_MUTABLE_EXT;
				DESCRIPTOR_TYPE_INLINE_UNIFORM_BLOCK_EXT = VK_DESCRIPTOR_TYPE_INLINE_UNIFORM_BLOCK_EXT;
				DESCRIPTOR_TYPE_MUTABLE_VALVE = VK_DESCRIPTOR_TYPE_MUTABLE_VALVE;
				DESCRIPTOR_TYPE_MAX_ENUM = VK_DESCRIPTOR_TYPE_MAX_ENUM;
				break;
			case DIRECT3D12:
				break;
			}
		}

		uint32_t 
			DESCRIPTOR_TYPE_SAMPLER{ 0 },
			DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER{ 0 },
			DESCRIPTOR_TYPE_SAMPLED_IMAGE{ 0 },
			DESCRIPTOR_TYPE_STORAGE_IMAGE{ 0 },
			DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER{ 0 },
			DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER{ 0 },
			DESCRIPTOR_TYPE_UNIFORM_BUFFER{ 0 },
			DESCRIPTOR_TYPE_STORAGE_BUFFER{ 0 },
			DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC{ 0 },
			DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC{ 0 },
			DESCRIPTOR_TYPE_INPUT_ATTACHMENT{ 0 },
			DESCRIPTOR_TYPE_INLINE_UNIFORM_BLOCK{ 0 },
			DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_KHR{ 0 },
			DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_NV{ 0 },
			DESCRIPTOR_TYPE_SAMPLE_WEIGHT_IMAGE_QCOM{ 0 },
			DESCRIPTOR_TYPE_BLOCK_MATCH_IMAGE_QCOM{ 0 },
			DESCRIPTOR_TYPE_MUTABLE_EXT{ 0 },
			DESCRIPTOR_TYPE_INLINE_UNIFORM_BLOCK_EXT{ 0 },
			DESCRIPTOR_TYPE_MUTABLE_VALVE{ 0 },
			DESCRIPTOR_TYPE_MAX_ENUM{ 0 };
	}DynamicDescriptorType;

	typedef struct DynamicImageLayout 
	{
		DynamicImageLayout(API api)
		{
			switch (api)
			{
			case VULKAN:
				IMAGE_LAYOUT_UNDEFINED = VK_IMAGE_LAYOUT_UNDEFINED;
				IMAGE_LAYOUT_GENERAL = VK_IMAGE_LAYOUT_GENERAL;
				IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
				IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
				IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL = VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;
				IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
				IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
				IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
				IMAGE_LAYOUT_PREINITIALIZED = VK_IMAGE_LAYOUT_PREINITIALIZED;
				IMAGE_LAYOUT_DEPTH_READ_ONLY_STENCIL_ATTACHMENT_OPTIMAL = VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_STENCIL_ATTACHMENT_OPTIMAL;
				IMAGE_LAYOUT_DEPTH_ATTACHMENT_STENCIL_READ_ONLY_OPTIMAL = VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_STENCIL_READ_ONLY_OPTIMAL;
				IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL = VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL;
				IMAGE_LAYOUT_DEPTH_READ_ONLY_OPTIMAL = VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_OPTIMAL;
				IMAGE_LAYOUT_STENCIL_ATTACHMENT_OPTIMAL = VK_IMAGE_LAYOUT_STENCIL_ATTACHMENT_OPTIMAL;
				IMAGE_LAYOUT_STENCIL_READ_ONLY_OPTIMAL = VK_IMAGE_LAYOUT_STENCIL_READ_ONLY_OPTIMAL;
				IMAGE_LAYOUT_READ_ONLY_OPTIMAL = VK_IMAGE_LAYOUT_READ_ONLY_OPTIMAL;
				IMAGE_LAYOUT_ATTACHMENT_OPTIMAL = VK_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL;
				IMAGE_LAYOUT_PRESENT_SRC_KHR = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
				IMAGE_LAYOUT_VIDEO_DECODE_DST_KHR = VK_IMAGE_LAYOUT_VIDEO_DECODE_DST_KHR;
				IMAGE_LAYOUT_VIDEO_DECODE_SRC_KHR = VK_IMAGE_LAYOUT_VIDEO_DECODE_SRC_KHR;
				IMAGE_LAYOUT_VIDEO_DECODE_DPB_KHR = VK_IMAGE_LAYOUT_VIDEO_DECODE_DPB_KHR;
				IMAGE_LAYOUT_SHARED_PRESENT_KHR = VK_IMAGE_LAYOUT_SHARED_PRESENT_KHR;
				IMAGE_LAYOUT_FRAGMENT_DENSITY_MAP_OPTIMAL_EXT = VK_IMAGE_LAYOUT_FRAGMENT_DENSITY_MAP_OPTIMAL_EXT;
				IMAGE_LAYOUT_FRAGMENT_SHADING_RATE_ATTACHMENT_OPTIMAL_KHR = VK_IMAGE_LAYOUT_FRAGMENT_SHADING_RATE_ATTACHMENT_OPTIMAL_KHR;
				IMAGE_LAYOUT_ATTACHMENT_FEEDBACK_LOOP_OPTIMAL_EXT = VK_IMAGE_LAYOUT_ATTACHMENT_FEEDBACK_LOOP_OPTIMAL_EXT;
				IMAGE_LAYOUT_DEPTH_READ_ONLY_STENCIL_ATTACHMENT_OPTIMAL_KHR = VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_STENCIL_ATTACHMENT_OPTIMAL_KHR;
				IMAGE_LAYOUT_DEPTH_ATTACHMENT_STENCIL_READ_ONLY_OPTIMAL_KHR = VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_STENCIL_READ_ONLY_OPTIMAL_KHR;
				IMAGE_LAYOUT_SHADING_RATE_OPTIMAL_NV = VK_IMAGE_LAYOUT_SHADING_RATE_OPTIMAL_NV;
				IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL_KHR = VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL_KHR;
				IMAGE_LAYOUT_DEPTH_READ_ONLY_OPTIMAL_KHR = VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_OPTIMAL_KHR;
				IMAGE_LAYOUT_STENCIL_ATTACHMENT_OPTIMAL_KHR = VK_IMAGE_LAYOUT_STENCIL_ATTACHMENT_OPTIMAL_KHR;
				IMAGE_LAYOUT_STENCIL_READ_ONLY_OPTIMAL_KHR = VK_IMAGE_LAYOUT_STENCIL_READ_ONLY_OPTIMAL_KHR;
				IMAGE_LAYOUT_READ_ONLY_OPTIMAL_KHR = VK_IMAGE_LAYOUT_READ_ONLY_OPTIMAL_KHR;
				IMAGE_LAYOUT_ATTACHMENT_OPTIMAL_KHR = VK_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL_KHR;
				IMAGE_LAYOUT_MAX_ENUM = VK_IMAGE_LAYOUT_MAX_ENUM;
				break;
			case DIRECT3D12:
				break;
			}
		}

		uint32_t
			IMAGE_LAYOUT_UNDEFINED{0},
			IMAGE_LAYOUT_GENERAL{ 0 },
			IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL{ 0 },
			IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL{ 0 },
			IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL{ 0 },
			IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL{ 0 },
			IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL{ 0 },
			IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL{ 0 },
			IMAGE_LAYOUT_PREINITIALIZED{ 0 },
			IMAGE_LAYOUT_DEPTH_READ_ONLY_STENCIL_ATTACHMENT_OPTIMAL{ 0 },
			IMAGE_LAYOUT_DEPTH_ATTACHMENT_STENCIL_READ_ONLY_OPTIMAL{ 0 },
			IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL{ 0 },
			IMAGE_LAYOUT_DEPTH_READ_ONLY_OPTIMAL{ 0 },
			IMAGE_LAYOUT_STENCIL_ATTACHMENT_OPTIMAL{ 0 },
			IMAGE_LAYOUT_STENCIL_READ_ONLY_OPTIMAL{ 0 },
			IMAGE_LAYOUT_READ_ONLY_OPTIMAL{ 0 },
			IMAGE_LAYOUT_ATTACHMENT_OPTIMAL{ 0 },
			IMAGE_LAYOUT_PRESENT_SRC_KHR{ 0 },
			IMAGE_LAYOUT_VIDEO_DECODE_DST_KHR{ 0 },
			IMAGE_LAYOUT_VIDEO_DECODE_SRC_KHR{ 0 },
			IMAGE_LAYOUT_VIDEO_DECODE_DPB_KHR{ 0 },
			IMAGE_LAYOUT_SHARED_PRESENT_KHR{ 0 },
			IMAGE_LAYOUT_FRAGMENT_DENSITY_MAP_OPTIMAL_EXT{ 0 },
			IMAGE_LAYOUT_FRAGMENT_SHADING_RATE_ATTACHMENT_OPTIMAL_KHR{ 0 },
			IMAGE_LAYOUT_ATTACHMENT_FEEDBACK_LOOP_OPTIMAL_EXT{ 0 },
			IMAGE_LAYOUT_DEPTH_READ_ONLY_STENCIL_ATTACHMENT_OPTIMAL_KHR{ 0 },
			IMAGE_LAYOUT_DEPTH_ATTACHMENT_STENCIL_READ_ONLY_OPTIMAL_KHR{ 0 },
			IMAGE_LAYOUT_SHADING_RATE_OPTIMAL_NV{ 0 },
			IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL_KHR{ 0 },
			IMAGE_LAYOUT_DEPTH_READ_ONLY_OPTIMAL_KHR{ 0 },
			IMAGE_LAYOUT_STENCIL_ATTACHMENT_OPTIMAL_KHR{ 0 },
			IMAGE_LAYOUT_STENCIL_READ_ONLY_OPTIMAL_KHR{ 0 },
			IMAGE_LAYOUT_READ_ONLY_OPTIMAL_KHR{ 0 },
			IMAGE_LAYOUT_ATTACHMENT_OPTIMAL_KHR{ 0 },
			IMAGE_LAYOUT_MAX_ENUM{ 0 };

	}DynamicImageLayout;

	typedef struct DynamicShaderStageFlagBits
	{
		DynamicShaderStageFlagBits(API api)
		{
			switch (api)
			{
			case VULKAN:
				SHADER_STAGE_VERTEX_BIT = VK_SHADER_STAGE_VERTEX_BIT;
				SHADER_STAGE_TESSELLATION_CONTROL_BIT = VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
				SHADER_STAGE_TESSELLATION_EVALUATION_BIT = VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
				SHADER_STAGE_GEOMETRY_BIT = VK_SHADER_STAGE_GEOMETRY_BIT;
				SHADER_STAGE_FRAGMENT_BIT = VK_SHADER_STAGE_FRAGMENT_BIT;
				SHADER_STAGE_COMPUTE_BIT = VK_SHADER_STAGE_COMPUTE_BIT;
				SHADER_STAGE_ALL_GRAPHICS = VK_SHADER_STAGE_ALL_GRAPHICS;
				SHADER_STAGE_ALL = VK_SHADER_STAGE_ALL;
				SHADER_STAGE_RAYGEN_BIT_KHR = VK_SHADER_STAGE_RAYGEN_BIT_KHR;
				SHADER_STAGE_ANY_HIT_BIT_KHR = VK_SHADER_STAGE_ANY_HIT_BIT_KHR;
				SHADER_STAGE_CLOSEST_HIT_BIT_KHR = VK_SHADER_STAGE_CLOSEST_HIT_BIT_KHR;
				SHADER_STAGE_MISS_BIT_KHR = VK_SHADER_STAGE_MISS_BIT_KHR;
				SHADER_STAGE_INTERSECTION_BIT_KHR = VK_SHADER_STAGE_INTERSECTION_BIT_KHR;
				SHADER_STAGE_CALLABLE_BIT_KHR = VK_SHADER_STAGE_CALLABLE_BIT_KHR;
				SHADER_STAGE_TASK_BIT_EXT = VK_SHADER_STAGE_TASK_BIT_EXT;
				SHADER_STAGE_MESH_BIT_EXT = VK_SHADER_STAGE_MESH_BIT_EXT;
				SHADER_STAGE_SUBPASS_SHADING_BIT_HUAWEI = VK_SHADER_STAGE_SUBPASS_SHADING_BIT_HUAWEI;
				SHADER_STAGE_CLUSTER_CULLING_BIT_HUAWEI = VK_SHADER_STAGE_CLUSTER_CULLING_BIT_HUAWEI;
				SHADER_STAGE_RAYGEN_BIT_NV = VK_SHADER_STAGE_RAYGEN_BIT_NV;
				SHADER_STAGE_ANY_HIT_BIT_NV = VK_SHADER_STAGE_ANY_HIT_BIT_NV;
				SHADER_STAGE_CLOSEST_HIT_BIT_NV = VK_SHADER_STAGE_CLOSEST_HIT_BIT_NV;
				SHADER_STAGE_MISS_BIT_NV = VK_SHADER_STAGE_MISS_BIT_NV;
				SHADER_STAGE_INTERSECTION_BIT_NV = VK_SHADER_STAGE_INTERSECTION_BIT_NV;
				SHADER_STAGE_CALLABLE_BIT_NV = VK_SHADER_STAGE_CALLABLE_BIT_NV;
				SHADER_STAGE_TASK_BIT_NV = VK_SHADER_STAGE_TASK_BIT_NV;
				SHADER_STAGE_MESH_BIT_NV = VK_SHADER_STAGE_MESH_BIT_NV;
				SHADER_STAGE_FLAG_BITS_MAX_ENUM = VK_SHADER_STAGE_FLAG_BITS_MAX_ENUM;
				break;

			case DIRECT3D12:
				break;
			}
		}

		uint32_t
			SHADER_STAGE_VERTEX_BIT{0},
			SHADER_STAGE_TESSELLATION_CONTROL_BIT{ 0 },
			SHADER_STAGE_TESSELLATION_EVALUATION_BIT{ 0 },
			SHADER_STAGE_GEOMETRY_BIT{ 0 },
			SHADER_STAGE_FRAGMENT_BIT{ 0 },
			SHADER_STAGE_COMPUTE_BIT{ 0 },
			SHADER_STAGE_ALL_GRAPHICS{ 0 },
			SHADER_STAGE_ALL{ 0 },
			SHADER_STAGE_RAYGEN_BIT_KHR{ 0 },
			SHADER_STAGE_ANY_HIT_BIT_KHR{ 0 },
			SHADER_STAGE_CLOSEST_HIT_BIT_KHR{ 0 },
			SHADER_STAGE_MISS_BIT_KHR{ 0 },
			SHADER_STAGE_INTERSECTION_BIT_KHR{ 0 },
			SHADER_STAGE_CALLABLE_BIT_KHR{ 0 },
			SHADER_STAGE_TASK_BIT_EXT{ 0 },
			SHADER_STAGE_MESH_BIT_EXT{ 0 },
			SHADER_STAGE_SUBPASS_SHADING_BIT_HUAWEI{ 0 },
			SHADER_STAGE_CLUSTER_CULLING_BIT_HUAWEI{ 0 },
			SHADER_STAGE_RAYGEN_BIT_NV{ 0 },
			SHADER_STAGE_ANY_HIT_BIT_NV{ 0 },
			SHADER_STAGE_CLOSEST_HIT_BIT_NV{ 0 },
			SHADER_STAGE_MISS_BIT_NV{ 0 },
			SHADER_STAGE_INTERSECTION_BIT_NV{ 0 },
			SHADER_STAGE_CALLABLE_BIT_NV{ 0 },
			SHADER_STAGE_TASK_BIT_NV{ 0 },
			SHADER_STAGE_MESH_BIT_NV{ 0 },
			SHADER_STAGE_FLAG_BITS_MAX_ENUM{ 0 };

	}DynamicShaderStageFlagBits;
	typedef DynamicShaderStageFlagBits DynamicShaderStageFlags;

	typedef struct DynamicMemoryPropertyFlagBits
	{
		DynamicMemoryPropertyFlagBits(API api)
		{
			switch (api)
			{
			case VULKAN:
				MEMORY_PROPERTY_DEVICE_LOCAL_BIT = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
				MEMORY_PROPERTY_HOST_VISIBLE_BIT = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;
				MEMORY_PROPERTY_HOST_COHERENT_BIT = VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
				MEMORY_PROPERTY_HOST_CACHED_BIT = VK_MEMORY_PROPERTY_HOST_CACHED_BIT;
				MEMORY_PROPERTY_LAZILY_ALLOCATED_BIT = VK_MEMORY_PROPERTY_LAZILY_ALLOCATED_BIT;
				MEMORY_PROPERTY_PROTECTED_BIT = VK_MEMORY_PROPERTY_PROTECTED_BIT;
				MEMORY_PROPERTY_DEVICE_COHERENT_BIT_AMD = VK_MEMORY_PROPERTY_DEVICE_COHERENT_BIT_AMD;
				MEMORY_PROPERTY_DEVICE_UNCACHED_BIT_AMD = VK_MEMORY_PROPERTY_DEVICE_UNCACHED_BIT_AMD;
				MEMORY_PROPERTY_RDMA_CAPABLE_BIT_NV = VK_MEMORY_PROPERTY_RDMA_CAPABLE_BIT_NV;
				MEMORY_PROPERTY_FLAG_BITS_MAX_ENUM = VK_MEMORY_PROPERTY_FLAG_BITS_MAX_ENUM;
				break;
			case DIRECT3D12:
				break;
			}
		}

		uint32_t
			MEMORY_PROPERTY_DEVICE_LOCAL_BIT{ 0 },
			MEMORY_PROPERTY_HOST_VISIBLE_BIT{ 0 },
			MEMORY_PROPERTY_HOST_COHERENT_BIT{ 0 },
			MEMORY_PROPERTY_HOST_CACHED_BIT{ 0 },
			MEMORY_PROPERTY_LAZILY_ALLOCATED_BIT{ 0 },
			MEMORY_PROPERTY_PROTECTED_BIT{ 0 },
			MEMORY_PROPERTY_DEVICE_COHERENT_BIT_AMD{ 0 },
			MEMORY_PROPERTY_DEVICE_UNCACHED_BIT_AMD{ 0 },
			MEMORY_PROPERTY_RDMA_CAPABLE_BIT_NV{ 0 },
			MEMORY_PROPERTY_FLAG_BITS_MAX_ENUM{ 0 };

	}DynamicMemoryPropertyFlagBits;

	typedef  DynamicMemoryPropertyFlagBits DynamicMemoryPropertyFlags;

	typedef struct DynamicDescriptorImageInfo {
		DynamicSampler        sampler;
		DynamicImageView      imageView;
		uint32_t              imageLayout;
	} DynamicDescriptorImageInfo;

	typedef struct DynamicWriteDescriptorSet
	{
		uint32_t                           dstBinding;
		uint32_t                           dstArrayElement;
		uint32_t                           descriptorCount;
		uint32_t                           descriptorType;
		DynamicDescriptorImageInfo*        pImageInfo;
		DynamicDescriptorBufferInfo*       pBufferInfo;
	}DynamicWriteDescriptorSet;

	typedef struct DynamicDescriptorSetLayoutBinding
	{
		uint32_t          binding;
		uint32_t          descriptorType;
		uint32_t          descriptorCount;
		uint32_t          stageFlags;
		DynamicSampler*   pImmutableSamplers;
	}DynamicDescriptorSetLayoutBinding;

	typedef struct DynamicPushConstantRange
	{
		uint32_t              stageFlags;
		uint32_t              offset;
		uint32_t              size;
	}DynamicPushConstantRange;

	typedef struct DynamicPipelineLayoutCreateInfo
	{
		const void*                     pNext;
		uint32_t                        setLayoutCount;
		DynamicDescriptorSetLayout*     pSetLayouts;
		uint32_t                        pushConstantRangeCount;
		DynamicPushConstantRange*       pPushConstantRanges;
	}DynamicPipelineLayoutCreateInfo;

	typedef struct DynamicPipelineCreateInfo
	{
		const char* vertexShader;
		const char* fragmentShader;
		DynamicVertexInputBindingDescription vertexInputBinding;
		std::vector<DynamicVertexInputAttributeDescription> vertexInputAttributes;
	}DynamicPipelineCreateInfo;

	typedef struct DynamicDescriptorPoolSize {
		uint32_t    type;
		uint32_t    descriptorCount;
	} DynamicDescriptorPoolSize;

	typedef struct DynamicDescriptorPoolCreateFlag
	{
		DynamicDescriptorPoolCreateFlag(API api)
		{
			switch (api)
			{
			case VULKAN:
				DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
				DESCRIPTOR_POOL_CREATE_UPDATE_AFTER_BIND_BIT = VK_DESCRIPTOR_POOL_CREATE_UPDATE_AFTER_BIND_BIT;
				DESCRIPTOR_POOL_CREATE_HOST_ONLY_BIT_EXT = VK_DESCRIPTOR_POOL_CREATE_HOST_ONLY_BIT_EXT;
				DESCRIPTOR_POOL_CREATE_UPDATE_AFTER_BIND_BIT_EXT = VK_DESCRIPTOR_POOL_CREATE_UPDATE_AFTER_BIND_BIT_EXT;
				DESCRIPTOR_POOL_CREATE_HOST_ONLY_BIT_VALVE = VK_DESCRIPTOR_POOL_CREATE_HOST_ONLY_BIT_VALVE;
				DESCRIPTOR_POOL_CREATE_FLAG_BITS_MAX_ENUM = VK_DESCRIPTOR_POOL_CREATE_FLAG_BITS_MAX_ENUM;
				break;
			case DIRECT3D12:
				break;
			}
		}

		uint32_t
			DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT{ 0 },
			DESCRIPTOR_POOL_CREATE_UPDATE_AFTER_BIND_BIT{ 0 },
			DESCRIPTOR_POOL_CREATE_HOST_ONLY_BIT_EXT{ 0 },
			DESCRIPTOR_POOL_CREATE_UPDATE_AFTER_BIND_BIT_EXT{ 0 },
			DESCRIPTOR_POOL_CREATE_HOST_ONLY_BIT_VALVE{ 0 },
			DESCRIPTOR_POOL_CREATE_FLAG_BITS_MAX_ENUM{ 0 };
	}DynamicDescriptorPoolCreateFlag;

	typedef struct DynamicDescriptorPoolCreateInfo
	{
		VkDescriptorPoolCreateFlags    flags;
		uint32_t                       maxSets;
		std::vector<DynamicDescriptorPoolSize>* pPoolSizes;
	}DynamicDescriptorPoolCreateInfo;

	typedef struct DynamicRenderingInfo
	{
		bool              isRenderOnSwapChain;
		uint32_t          swapChainIndex;
		bool              isClearEveryFrame;
		//if not render on swap chain, using target iamge and iamge view
		DynamicImage*     targetImage;
		DynamicImageView* targetImageView;
	}DynamicRenderingInfo;
}




