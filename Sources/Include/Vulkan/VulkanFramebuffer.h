#pragma once

#include"../InterfaceType.h"

namespace DRHI
{
	namespace VulkanFramebuffer
	{
		void createFramebuffer(DynamicFramebuffer* framebuffer, DynamicFramebufferCreateInfo* fci, VkDevice device);
	}
}