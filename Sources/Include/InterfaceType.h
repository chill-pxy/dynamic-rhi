#pragma once

#define VK_USE_PLATFORM_WIN32_KHR
#include<volk.h>
#include<Windows.h>

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

const int MAX_FRAMES_IN_FLIGHT = 2;

namespace DRHI
{
	typedef enum API
	{
		VULKAN,
		DIRECT3D12
	}API;
}




