#pragma once

#include<variant>
#include<iostream>

#include"InterfaceType.h"
#include"Device.h"

namespace DRHI
{
	class DescriptorSet
	{
	private:
		std::variant<VkDescriptorSet*> _runtimeDescriptorSet;

	public:
		//void createDescriptorSet(Device* pdevice);

	public:

		DescriptorSet()
		{
			_runtimeDescriptorSet = new VkDescriptorSet();
		}

		DescriptorSet(API api)
		{
			switch (api)
			{
			case API::VULKAN:
				_runtimeDescriptorSet = new VkDescriptorSet();
				break;
			}

		}

		VkDescriptorSet* getVkDescriptorSet()
		{
			if (std::holds_alternative<VkDescriptorSet*>(_runtimeDescriptorSet))
			{
				return std::get<VkDescriptorSet*>(_runtimeDescriptorSet);
			}
			else
			{
				std::cout << "none vk DescriptorSet";
				return nullptr;
			}
		}
	};
}
