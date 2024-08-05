#pragma once

#include<variant>

#include"Instance.h"


namespace DRHI
{
	class PhysicalDevice
	{
	private:
		std::variant<VkPhysicalDevice*,int> _runtimePhysicalDevice;

	public:
		void pickPhysicalDevice(unsigned int id, Instance* instance);

	public:
		
		PhysicalDevice()
		{
			_runtimePhysicalDevice = new VkPhysicalDevice();
		}

		PhysicalDevice(API api)
		{
			switch (api)
			{
			case API::VULKAN:
				_runtimePhysicalDevice = new VkPhysicalDevice();
				break;
			}
		}
	};
}
