#include "../../Include/InterfaceConfig.h"

#ifdef VULKAN_IMPLEMENTATION

#include "../../Include/Context.h"

namespace DRHI
{
	void Context::createSurface(Instance* pinstance, GLFWwindow* window, Surface* psurface)
	{
		VkSurfaceKHR surface = VkSurfaceKHR();

		auto instance = _instance->getVkInstance();

		if (glfwCreateWindowSurface(*instance, window, nullptr, &surface) != VK_SUCCESS) {
			throw std::runtime_error("failed to create window surface!");
		}
	}
}

#endif