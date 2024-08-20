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

enum API
{
	VULKAN,
	DIRECT3D12
};

enum VertexInputRate
{
	VERTEX_INPUT_RATE_VERTEX = 0,
	VERTEX_INPUT_RATE_INSTANCE = 1,
	VERTEX_INPUT_RATE_MAX_ENUM = 0x7FFFFFFF
};

enum Format
{

};
