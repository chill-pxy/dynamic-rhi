#include "../Sources/Include/Context.h"
#include "../Sources/Include/Instance.h"

using namespace DRHI;

int main()
{
	ContextCreatInfo info = {
		API::VULKAN,
		"DRHI Test Application",
		1920,
		1080
	};

	Context* context = new Context(info);
	context->initialize();

	return 0;
}