#pragma once

#include<variant>
#include<iostream>

#include"InterfaceType.h"
#include"Device.h"

namespace DRHI
{
	class DescriptorSetLayout
	{
	private:
		std::variant<VkDescriptorSetLayout*> _runtimeDscriptorSetLayout;

	public:
		void createDescriptorSetLayout(Device* pdevice);

	public:

		DescriptorSetLayout()
		{
			_runtimeDscriptorSetLayout = new VkDescriptorSetLayout();
		}

		DescriptorSetLayout(API api)
		{
			switch (api)
			{
			case API::VULKAN:
				_runtimeDscriptorSetLayout = new VkDescriptorSetLayout();
				break;
			}

		}

		VkDescriptorSetLayout* getVkDescriptorSetLayout()
		{
			if (std::holds_alternative<VkDescriptorSetLayout*>(_runtimeDscriptorSetLayout))
			{
				return std::get<VkDescriptorSetLayout*>(_runtimeDscriptorSetLayout);
			}
			else
			{
				std::cout << "none vk DescriptorPool";
				return nullptr;
			}
		}
	};
}
