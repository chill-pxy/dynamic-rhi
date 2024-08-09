#include "../../Include/InterfaceConfig.h"

#ifdef VULKAN_IMPLEMENTATION

#include "../../Include/NativeWindow.h"

namespace DRHI
{
	void NativeWindow::initialize(const char* title, int width, int height)
	{
		glfwInit();
		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions;
		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		_extensions = std::vector<const char*> (glfwExtensions, glfwExtensions + glfwExtensionCount);

		if (enableValidationLayers)
		{
			_extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		}

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		if (!glfwVulkanSupported())
		{
			printf("GLFW: Vulkan Not Supported\n");
		}
		_runtimeWindow = glfwCreateWindow(width, height, title, nullptr, nullptr);
	}
}

#endif