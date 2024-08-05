#include "../../Include/InterfaceConfig.h"

#ifdef DIRECT3D12_IMPLEMENTATION

#include <iostream>

#include "../../Include/Instance.h"

namespace DRHI
{
	void Instance::createInstance()
	{
		std::cout<<"DX12 Instance" << std::endl;
	}
}


#endif