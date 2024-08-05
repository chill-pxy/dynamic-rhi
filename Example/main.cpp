#include "../Sources/Include/Context.h"

using namespace DRHI;

int main()
{
	Context* context = new Context();
	context->initialize();

	return 0;
}