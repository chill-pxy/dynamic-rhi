#pragma once
#include<variant>

#include"InterfaceType.h"
#include"Device.h"

namespace DRHI
{
	class CommandPool
	{
	private:
		std::variant<VkCommandPool*> _runtimeCommandPool;

	public:
		void createCommandPool(Device* pdevice);

	public:
		CommandPool()
		{
			_runtimeCommandPool = new VkCommandPool();
		}

		CommandPool(API api)
		{
			switch (api)
			{
			case VULKAN:
				_runtimeCommandPool = new VkCommandPool();
				break;
			case DIRECT3D12:
				break;
			default:
				break;
			}
		}


	};
}