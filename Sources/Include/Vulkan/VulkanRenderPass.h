#pragma once

#include"../InterfaceType.h"

namespace drhi
{
	namespace VulkanRenderPass
	{
		void createRenderPass(DynamicRenderPass* renderPass, DynamicRenderPassCreateInfo* ci, VkDevice device);
	}
}