#include"../Sources/Include/Vulkan/VulkanDRHI.h"
#include"NativeWindow.h"

using namespace DRHI;

int main()
{
    DynamicRHI* _platformContext;

    FOCUS::NativeWindowCreateInfo windowInfo = {
        "Dynamic RHI",
        1920,
        1080
    };

    auto nwindow = new FOCUS::NativeWindow(windowInfo);
    nwindow->initialize();

    DRHI::PlatformInfo pCi{};
    pCi.window = nwindow->_hwnd;
    pCi.width = windowInfo.width;
    pCi.height = windowInfo.height;

    DRHI::RHICreateInfo rhiCi{};
    rhiCi.platformInfo = pCi;

    _platformContext = new VulkanDRHI(rhiCi);
    _platformContext->initialize();

    PipelineCreateInfo pci = {};
    pci.vertexShader = "../../../Example/shaders/model_vertex.spv";
    pci.fragmentShader = "../../../Example/shaders/model_fragment.spv";

    static_cast<VulkanDRHI*>(_platformContext)->createPipeline(pci);
   // _platformContext->prepareCommandBuffer();

    bool running = true;

    while (running)
    {
        running = nwindow->tick();
        _platformContext->frameOnTick();
    }

    _platformContext->clean();


	return 0;
}