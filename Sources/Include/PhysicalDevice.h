#pragma once

#include<variant>

#include"Instance.h"


namespace DRHI
{
	struct LUID
	{
		uint8_t data[8] = {};
	};

	struct PhysicalDeviceDesc
	{
		wchar_t  Description[128];
		unsigned long long DedicatedVideoMemory;
		unsigned long long SharedSystemMemory;
		LUID   AdapterLuid;
	};

	class PhysicalDevice
	{
	private:
		std::variant<VkPhysicalDevice*,int> _runtimePhysicalDevice;
		uint32_t _queueFamily = (uint32_t)-1;

	public:
		void pickPhysicalDevice(unsigned int id, Instance* instance);
		void pickGraphicQueueFamily();

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

		VkPhysicalDevice* getVkPhysicalDevice()
		{
			if (std::holds_alternative<VkPhysicalDevice*>(_runtimePhysicalDevice))
			{
				return std::get<VkPhysicalDevice*>(_runtimePhysicalDevice);
			}
			else
			{
				std::cout << "none vk physical device";
				return nullptr;
			}
		}

		uint32_t getQueueFamily()
		{
			return _queueFamily;
		}
	};
}
