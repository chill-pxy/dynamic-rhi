#include "../Sources/Include/Context.h"

using namespace DRHI;

int main()
{
	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	GLFWwindow* window = glfwCreateWindow(1920, 1080, "FOCUS", nullptr, nullptr);

	Context* context = new Context();
	context->initialize(window);

	return 0;
}