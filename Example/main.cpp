#include "../Sources/Include/Context.h"
#include "../Sources/Include/Instance.h"

using namespace DRHI;

class WindowsSurface
{
public:
    void init()
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

        _window = glfwCreateWindow(800, 800, "focus", nullptr, nullptr);
    }

    void cleanup()
    {
        glfwDestroyWindow(_window);

        glfwTerminate();
    }

    void update()
    {
        glfwPollEvents();
    }

    GLFWwindow* getWindowInstance()
    {
        return _window;
    }

    std::vector<const char*> getExtensions()
    {
        return _extensions;
    }


    bool checkForClose()
    {
        return glfwWindowShouldClose(_window);
    }

private:
    GLFWwindow* _window;
    std::vector<const char*> _extensions;
};

int main()
{
	std::unique_ptr<Context> _platformContext;
	std::unique_ptr<WindowsSurface> _windowContext;

	_windowContext = std::make_unique<WindowsSurface>();
	_windowContext->init();

	DRHI::ContextCreatInfo info = {
		API::VULKAN,
		_windowContext->getWindowInstance(),
		_windowContext->getExtensions()
	};

	_platformContext = std::make_unique<DRHI::Context>(info);

    while (!_windowContext->checkForClose())
    {
        _windowContext->update();
    }

    _platformContext->initialize();

    _windowContext->cleanup();

	return 0;
}