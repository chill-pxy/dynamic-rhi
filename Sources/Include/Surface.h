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

		//specify api type to create surface
		void createSurface(API api, Instance* pinstance, GLFWwindow* window)
		{
			switch (api)
			{
			case VULKAN:
				createVkSurface(pinstance, window);
				break;
			case DIRECT3D12:
				createDxSurface();
				break;
			default:
				createVkSurface(pinstance, window);
				break;
			}
		}

	private:
		//due to glfw can not be recongized as initilized state cross DLL source file, 
		// all glfw related functions should be called within header file 
		void createVkSurface(Instance* pinstance, GLFWwindow* window)
		{
			VkSurfaceKHR* surface = new VkSurfaceKHR();

			auto instance = pinstance->getVkInstance();

			if (glfwCreateWindowSurface(*instance, window, nullptr, surface) != VK_SUCCESS) {
				throw std::runtime_error("failed to create window surface!");
			}

			_runtimeSurface = surface;
		}

		void createDxSurface()
		{
			std::cout << "dx surface";
		}
	};
}