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
		std::variant<VkQueue*> _graphicQueue;

	public:

		CommandQueue()
		{
			_graphicQueue = new VkQueue();
		}

		CommandQueue(API api)
		{
			switch (api)
			{
			case API::VULKAN:
				_graphicQueue = new VkQueue();
				break;
			}

		}

		VkQueue* getVkGraphicQueue()
		{
			if (std::holds_alternative<VkQueue*>(_graphicQueue))
			{
				return std::get<VkQueue*>(_graphicQueue);
			}
			else
			{
				std::cout << "none vk graphic queue";
				return nullptr;
			}
		}
	};
}
