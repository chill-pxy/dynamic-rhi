#pragma once

#include"../InterfaceType.h"

namespace DRHI
{
	namespace VulkanRenderPass
	{
		void createRenderPass(DynamicRenderPass* renderPass, DynamicRenderPassCreateInfo* ci, VkDevice device);
	}
}