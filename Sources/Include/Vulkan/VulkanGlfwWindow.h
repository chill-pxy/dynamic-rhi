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

	class VulkanGlfwWindow
	{
	private:
		std::vector<const char*> _extensions;
		GLFWwindow* _window;

	public:
		VulkanGlfwWindow() = default;

		void initialize(VulkanGlfwWindowCreateInfo glfwWindowCreateInfo)
		{
			glfwInit();
			uint32_t glfwExtensionCount = 0;
			const char** glfwExtensions;
			glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

			_extensions = std::vector<const char*>(glfwExtensions, glfwExtensions + glfwExtensionCount);

			_extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

			glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
			if (!glfwVulkanSupported())
			{
				printf("GLFW: Vulkan Not Supported\n");
			}

			_window = glfwCreateWindow(glfwWindowCreateInfo.width, glfwWindowCreateInfo.height, glfwWindowCreateInfo.titleName,nullptr, nullptr);
		}

		GLFWwindow* getVulkanGlfwWindow()
		{
			return _window;
		}

		std::vector<const char*> getVulkanGlfwWindowExtensions()
		{
			return _extensions;
		}
	};
}