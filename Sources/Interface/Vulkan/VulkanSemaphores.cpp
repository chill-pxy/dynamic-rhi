#include<stdexcept>

#include "../../Include/Vulkan/VulkanSemphores.h"

namespace drhi
{
	namespace VulkanSemaphores
	{
		void createSemaphore(Semaphores* semaphores, VkDevice* device)
		{
			VkSemaphoreCreateInfo semaphoreCreateInfo{};
			semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

			if (vkCreateSemaphore(*device, &semaphoreCreateInfo, nullptr, &semaphores->presentComplete) != VK_SUCCESS)
			{
				throw std::runtime_error("failed to crete Semaphore");
			}

			if (vkCreateSemaphore(*device, &semaphoreCreateInfo, nullptr, &semaphores->renderComplete) != VK_SUCCESS)
			{
				throw std::runtime_error("failed to crete Semaphore");
			}
		}
	}
}