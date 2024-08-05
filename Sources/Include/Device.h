#pragma once

#include<variant>

#include"volk.h"

namespace DRHI
{
	class Device
	{
	private:
		std::variant<VkDevice*> _runtimeDevice;

	public:
		void CreateCommandAllocators();
		void CreateCommandList();
	};
}
