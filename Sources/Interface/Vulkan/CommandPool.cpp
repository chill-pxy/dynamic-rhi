#include "../../Include/CommandPool.h"

#ifdef VULKAN_IMPLEMENTATION

namespace DRHI
{
	void CommandPool::createCommandPool(Device* pdevice)
	{
		auto device = pdevice->getVkDevice();

		VkCommandPool* commandPool = new VkCommandPool();

		VkCommandPoolCreateInfo info{};
		info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		info.pNext = NULL;
		info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		info.queueFamilyIndex = pdevice->getQueueFamilyIndices().graphicsFamily.value();

		if (vkCreateCommandPool(*device, &info, nullptr, commandPool) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create command pool");
		}

		_runtimeCommandPool = commandPool;
	}
}

#endif