#pragma once

#include<vector>

#include"../InterfaceType.h"

namespace DRHI
{
	struct VulkanGlfwWindowCreateInfo
	{
		const char* titleName;
		unsigned int width;
		unsigned int height;
	};

	GLFWwindow* createGlfwWindow(std::vector<const char*>* extensions, VulkanGlfwWindowCreateInfo glfwWindowCreateInfo)
	{
		glfwInit();
		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions;
		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		*extensions = std::vector<const char*>(glfwExtensions, glfwExtensions + glfwExtensionCount);

		extensions->push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		if (!glfwVulkanSupported())
		{
			printf("GLFW: Vulkan Not Supported\n");
		}

		return glfwCreateWindow(glfwWindowCreateInfo.width, glfwWindowCreateInfo.height, glfwWindowCreateInfo.titleName,nullptr, nullptr);
	}

	void cleanVulkanGlfwWindow(GLFWwindow* window)
	{
		glfwDestroyWindow(window);

		glfwTerminate();
	}
}