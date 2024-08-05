#pragma once
#include<variant>
#include<iostream>

#include<volk.h>

#include"InterfaceType.h"

namespace FOCUS
{
	namespace Platform
	{
		class Context;

		class Instance
		{
		private:
			//int为测试类型，实际应设计为其他API的instance类型
			std::variant<VkInstance*, int> _runtimeInstance;

		public:
			void createInstance();
			void test();
			//void createSwapChain();
			//void getNumPhysicalDevices();
			//void getAllPhysicalDevices();
			//void getPhysicalDevice();
		
		public:
			Instance(API api)
			{
				switch (api)
				{
				case API::VULKAN:
					_runtimeInstance = new VkInstance();
					break;
				}
			}

			std::variant<VkInstance*, int> getInstance()
			{
				return _runtimeInstance;
			}
		};


		
	}
}