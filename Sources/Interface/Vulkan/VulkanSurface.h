#pragma once

#include<stdexcept>

#include"../../Include/InterfaceType.h"

namespace DRHI
{
	//due to glfw can not be recongized as initilized state cross DLL source file, 
	// all glfw related functions should be called within header file 
	void createSurface(VkSurfaceKHR* osurface, VkInstance* instance, GLFWwindow* window)
	{
		VkSurfaceKHR* surface = new VkSurfaceKHR();

		if (glfwCreateWindowSurface(*instance, window, nullptr, surface) != VK_SUCCESS) {
			throw std::runtime_error("failed to create window surface!");
		}

		osurface = surface;
	}
}