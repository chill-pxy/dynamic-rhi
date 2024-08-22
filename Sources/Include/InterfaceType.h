#pragma once

#include<volk.h>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include"InterfaceConfig.h"

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

namespace DRHI
{
	enum API
	{
		VULKAN,
		DIRECT3D12
	};

	enum VertexInputRate;

	enum Format;
}