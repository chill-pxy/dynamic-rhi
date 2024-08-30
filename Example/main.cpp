#include "../Sources/Include/Vulkan/VulkanDRHI.h"

using namespace DRHI;

int main()
{
    DynamicRHI* _platformContext;

    DRHI::VulkanGlfwWindowCreateInfo windowInfo = {
        "Dynamic RHI",
        1920,
        1080
    };

	DRHI::RHICreatInfo info = {
        windowInfo
	};

    _platformContext = new VulkanDRHI(info);
    _platformContext->initialize();

    auto window = dynamic_cast<VulkanDRHI*>(_platformContext)->_glfwWindow;

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
    }

    glfwDestroyWindow(window);

    glfwTerminate();

	return 0;
}