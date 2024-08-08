#pragma once

#include<volk.h>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>


#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

enum API
{
	VULKAN,
	DIRECT3D12
};