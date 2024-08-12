#pragma once

#include<variant>
#include<iostream>

#include"InterfaceType.h"
#include"Instance.h"

namespace DRHI
{
	class Surface
	{
	private:
		std::variant<VkSurfaceKHR*> _runtimeSurface;

	public:
		void createSurface(Instance* instance, GLFWwindow* window);

	public:
		
		Surface()
		{
			_runtimeSurface = new VkSurfaceKHR();
		}

		Surface(API api)
		{
			switch (api)
			{
			case API::VULKAN:
				_runtimeSurface = new VkSurfaceKHR();
				break;
			}
		}

		void setSurface(VkSurfaceKHR* surface)
		{
			_runtimeSurface = surface;
		}

		VkSurfaceKHR* getVkSurface()
		{
			if (std::holds_alternative<VkSurfaceKHR*>(_runtimeSurface))
			{
				return std::get<VkSurfaceKHR*>(_runtimeSurface);
			}
			else
			{
				std::cout << "none vk surface";
				return nullptr;
			}
		}
	};
}