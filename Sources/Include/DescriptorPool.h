#pragma once

#include<variant>
#include<iostream>

#include<volk.h>

#include"InterfaceType.h"

namespace DRHI
{
	class DescriptorPool
	{
	private:
		std::variant<VkDescriptorPool*> _runtimeDescriptorPool;

	public:
		

	public:

		DescriptorPool()
		{
			_runtimeDescriptorPool = new VkDescriptorPool();
		}

		DescriptorPool(API api)
		{
			switch (api)
			{
			case API::VULKAN:
				_runtimeDescriptorPool = new VkDescriptorPool();
				break;
			}

		}

		VkDescriptorPool* getVkDevice()
		{
			if (std::holds_alternative<VkDescriptorPool*>(_runtimeDescriptorPool))
			{
				return std::get<VkDescriptorPool*>(_runtimeDescriptorPool);
			}
			else
			{
				std::cout << "none vk DescriptorPool";
				return nullptr;
			}
		}
	};
}
