#include"../../Include/InterfaceConfig.h"

#ifdef VULKAN_IMPLEMENTATION

#include<array>

#include "../../Include/DescriptorPool.h"

const int MAX_FRAMES_IN_FLIGHT = 2;

namespace DRHI
{
	void DescriptorPool::createDescriptorPool(Device* pdevice)
	{
		VkDescriptorPool* descriptorPool = new VkDescriptorPool();
        auto device = pdevice->getVkDevice();
		
        std::array<VkDescriptorPoolSize, 2> poolSizes{};
        poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        poolSizes[0].descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
        poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        poolSizes[1].descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);

        VkDescriptorPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
        poolInfo.pPoolSizes = poolSizes.data();
        poolInfo.maxSets = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);

        if (vkCreateDescriptorPool(*device, &poolInfo, nullptr, descriptorPool) != VK_SUCCESS) {
            throw std::runtime_error("failed to create descriptor pool!");
        }

        _runtimeDescriptorPool = descriptorPool;
	}
}

#endif