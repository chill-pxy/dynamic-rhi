#include "../Sources/Include/Context.h"
#include "../Sources/Include/Instance.h"

using namespace DRHI;

int main()
{
	glfwInit();
	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions;
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

	if (enableValidationLayers)
	{
		extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	}

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	if (!glfwVulkanSupported())
	{
		printf("GLFW: Vulkan Not Supported\n");
	} 

	GLFWwindow* window = glfwCreateWindow(1920, 1080, "FOCUS", nullptr, nullptr);

	//VkSurfaceKHR surface = VkSurfaceKHR();

	//Instance* i = new Instance();
	//i->createInstance(extensions);
	//auto instance = i->getVkInstance();

	//if (glfwCreateWindowSurface(*instance, window, nullptr, &surface) != VK_SUCCESS) {
	//	throw std::runtime_error("failed to create window surface!");
	//}

	Context* context = new Context();
	context->initialize(window, extensions);

	return 0;
}