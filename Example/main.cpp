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

    PipelineCreateInfo pci = {};
    pci.vertexShader = "./shaders/model_vertex.spv";
    pci.fragmentShader = "./shaders/model_fragment.spv";

    dynamic_cast<VulkanDRHI*>(_platformContext)->createPipeline(pci);
    _platformContext->beginCommandBuffer();

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
    }

    _platformContext->clean();

    glfwDestroyWindow(window);

    glfwTerminate();

	return 0;
}