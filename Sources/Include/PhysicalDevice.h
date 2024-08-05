#pragma once

#include<variant>

#include"volk.h"

#include"Instance.h"


namespace DRHI
{
	class PhysicalDevice
	{
	private:
		std::variant<VkPhysicalDevice*> _runtimePhysicalDevice;

	public:
		void pickPhysicalDevice(unsigned int id, Instance* instance);
	};
}
