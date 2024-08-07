#pragma once

#include<variant>
#include<iostream>

#include<volk.h>

#include"InterfaceType.h"

namespace DRHI
{
	class CommandQueue
	{
	private:
		std::variant<VkQueue*> _runtimeQueue;

	public:

		CommandQueue()
		{
			_runtimeQueue = new VkQueue();
		}

		CommandQueue(API api)
		{
			switch (api)
			{
			case API::VULKAN:
				_runtimeQueue = new VkQueue();
				break;
			}

		}

		VkQueue* getVkQueue()
		{
			if (std::holds_alternative<VkQueue*>(_runtimeQueue))
			{
				return std::get<VkQueue*>(_runtimeQueue);
			}
			else
			{
				std::cout << "none vk queue";
				return nullptr;
			}
		}
	};
}
