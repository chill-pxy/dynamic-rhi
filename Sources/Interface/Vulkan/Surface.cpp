#include "../../Include/InterfaceConfig.h"

#ifdef VULKAN_IMPLEMENTATION

#include "../../Include/Surface.h"

namespace DRHI
{
    void Surface::createSurface(Instance* pinstance, GLFWwindow* window)
    {
        VkSurfaceKHR* surface = new VkSurfaceKHR();
        auto instance = pinstance->getVkInstance();

        if (glfwCreateWindowSurface(*instance, window, nullptr, surface) != VK_SUCCESS) {
            throw std::runtime_error("failed to create window surface!");
        }

        _runtimeSurface = surface;
    }
}

#endif
