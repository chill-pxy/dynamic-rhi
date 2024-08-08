#pragma once

#include<variant>
#include<iostream>

#include"PhysicalDevice.h"
#include"CommandQueue.h"
#include"Surface.h"

namespace DRHI
{
	class Device
	{
	private:
		std::variant<VkDevice*> _runtimeDevice;

	public:
		void createLogicalDevice(PhysicalDevice* phyDevice, CommandQueue* pgraphicQueue, CommandQueue* ppresentQueue, Surface* surface);

	public:

		Device()
		{
			_runtimeDevice = new VkDevice();
		}

		Device(API api)
		{
			switch (api)
			{
			case API::VULKAN:
				_runtimeDevice = new VkDevice();
				break;
			}
			
		}

		VkDevice* getVkDevice()
		{
			if (std::holds_alternative<VkDevice*>(_runtimeDevice))
			{
				return std::get<VkDevice*>(_runtimeDevice);
			}
			else
			{
				std::cout << "none vk device";
				return nullptr;
			}
		}
	};
}
