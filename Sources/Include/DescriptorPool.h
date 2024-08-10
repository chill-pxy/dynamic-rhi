#pragma once

#include<variant>
#include<iostream>

#include"InterfaceType.h"
#include"Device.h"

namespace DRHI
{
	class DescriptorPool
	{
	private:
		std::variant<VkDescriptorPool*> _runtimeDescriptorPool;
		std::variant<VkDescriptorSetLayout*> _runtimeDscriptorSetLayout;

	public:
		void createDescriptorPool(Device* pdevice);
		void createDescriptorSetLayout(Device* pdevice);

	public:

		DescriptorPool()
		{
			_runtimeDescriptorPool = new VkDescriptorPool();
			_runtimeDscriptorSetLayout = new VkDescriptorSetLayout();
		}

		DescriptorPool(API api)
		{
			switch (api)
			{
			case API::VULKAN:
				_runtimeDescriptorPool = new VkDescriptorPool();
				_runtimeDscriptorSetLayout = new VkDescriptorSetLayout();
				break;
			}

		}

		VkDescriptorPool* getVkDescriptorPool()
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
