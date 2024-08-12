#include "../Sources/Include/Context.h"
#include "../Sources/Include/Instance.h"

using namespace DRHI;

int main()
{
	//ContextCreatInfo info = {
	//	API::VULKAN,
	//	"DRHI Test Application",
	//	1920,
	//	1080
	//};

	/*Context* context = new Context(info);
	context->initialize();

	auto window = context->getNativeWindow();

	while (!window->checkForClose())
	{
		window->update();
	}

	window->cleanup();*/

	glfwInit();
	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions;
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	auto extensions = std::vector<const char*>(glfwExtensions, glfwExtensions + glfwExtensionCount);

	if (enableValidationLayers)
	{
		extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	}

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	if (!glfwVulkanSupported())
	{
		printf("GLFW: Vulkan Not Supported\n");
	}

	auto window = glfwCreateWindow(100, 100, "focus", nullptr, nullptr);

	DRHI::ContextCreatInfo info = {
		API::VULKAN,
		window,
		extensions
	};

	auto _platformContext = std::make_unique<DRHI::Context>(info);
	_platformContext->initialize();

	return 0;
}