#pragma once

#include<vector>
#include<stdexcept>

#include"../../Include/InterfaceType.h"

namespace drhi
{
	struct PlatformInfo
	{
		HWND window;
		uint32_t width;
		uint32_t height;
	};

	void createSurface(VkSurfaceKHR* surface, VkInstance* instance, PlatformInfo platformInfo);
}