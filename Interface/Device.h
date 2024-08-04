#pragma once

#include<variant>

#include"volk.h"

namespace FOCUS
{
	namespace Platform
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
}