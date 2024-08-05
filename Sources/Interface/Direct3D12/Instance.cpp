#include "../InterfaceConfig.h"

#ifdef DIRECT3D12_IMPLEMENTATION

#include <iostream>

#include "../Instance.h"

namespace FOCUS
{
	namespace Platform
	{
		void Instance::createInstance()
		{
			std::cout<<"DX12 Instance" << std::endl;
		}
	}
}

#endif