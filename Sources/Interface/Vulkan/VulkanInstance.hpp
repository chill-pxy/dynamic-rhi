#pragma once

#include"../../Include/InterfaceType.h"

namespace DRHI
{
	class VulkanInstance
	{
		VkInstance* createInstance(std::vector<const char*> extensions);
	};
}