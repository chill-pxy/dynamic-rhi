#include"../../Include/Vulkan/VulkanFence.h"

namespace DRHI
{
	void createSynchronizationPrimitives(std::vector<VkFence>* waitFences, uint32_t frameSize, VkDevice* device)
	{
		// Wait fences to sync command buffer access
		VkFenceCreateInfo fenceCreateInfo{};
		fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceCreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
		waitFences->resize(frameSize);
		for (auto& fence : *waitFences) 
		{
			if (vkCreateFence(*device, &fenceCreateInfo, nullptr, &fence) != VK_SUCCESS)
			{
				throw std::runtime_error("failed to create fence");
			}
		}
	}
}