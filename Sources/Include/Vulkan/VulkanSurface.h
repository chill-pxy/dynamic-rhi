#pragma once

#include<vector>
#include<stdexcept>

#include"../../Include/InterfaceType.h"

namespace DRHI
{
	//due to glfw can not be recongized as initilized state cross DLL source file, 
	// all glfw related functions should be called within header file 
	void createSurface(VkSurfaceKHR* surface, VkInstance* instance, GLFWwindow* window);
}