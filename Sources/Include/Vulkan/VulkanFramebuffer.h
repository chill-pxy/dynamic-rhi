#pragma once

#include"../InterfaceType.h"

namespace drhi
{
	namespace VulkanFramebuffer
	{
		void createFramebuffer(DynamicFramebuffer* framebuffer, DynamicFramebufferCreateInfo* fci, VkDevice device);
	}
}