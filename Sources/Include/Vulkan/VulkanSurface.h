#pragma once

#include<vector>
#include<stdexcept>

#include"../../Include/InterfaceType.h"

namespace DRHI
{
	struct PlatformInfo
	{
		HWND window;
	};

	void createSurface(VkSurfaceKHR* surface, VkInstance* instance, PlatformInfo platformInfo);
}