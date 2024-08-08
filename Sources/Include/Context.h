#pragma once
#include <memory>
#include <iostream>

#include "Instance.h"
#include "Device.h"
#include "PhysicalDevice.h"
#include "SwapChain.h"
#include "CommandQueue.h"
#include "Texture.h"
#include "DescriptorPool.h"
#include "InterfaceType.h"
#include "Surface.h"

namespace DRHI
{
	class Context
	{
	public:
		Context()
		{
			//ƒ¨»œ π”√Vulkan
			_runtimeInterface = API::VULKAN;

			_instance = std::make_unique<Instance>();
			_physicalDevice = std::make_unique<PhysicalDevice>();
			_device = std::make_unique<Device>();
			_graphicQueue = std::make_unique<CommandQueue>();
			_presentQueue = std::make_unique<CommandQueue>();
			_swapChain = std::make_unique<SwapChain>();
			_surface = std::make_unique<Surface>();
		}

		Context(API api)
		{
			_runtimeInterface = api;

			_instance = std::make_unique<Instance>(_runtimeInterface);
			_physicalDevice = std::make_unique<PhysicalDevice>(_runtimeInterface);
			_device = std::make_unique<Device>(_runtimeInterface);
			_graphicQueue = std::make_unique<CommandQueue>(_runtimeInterface);
			_presentQueue = std::make_unique<CommandQueue>(_runtimeInterface);
			_swapChain = std::make_unique<SwapChain>(_runtimeInterface);
			_surface = std::make_unique<Surface>(_runtimeInterface);
		}

		void initialize(GLFWwindow* window, std::vector<const char*> extensions)
		{
			_instance->createInstance(extensions);
			_surface->createSurface(_instance.get(), window);
			_physicalDevice->pickPhysicalDevice(0, _instance.get());
			_physicalDevice->pickGraphicQueueFamily();
			_device->createLogicalDevice(_physicalDevice.get(), _graphicQueue.get(), _presentQueue.get(), _surface.get());
			
			//_swapChain->createSwapChain(_physicalDevice.get(), _device.get(), _surface.get(), window);
		}

	private:
		API _runtimeInterface;

		std::unique_ptr<Instance> _instance;
		std::unique_ptr<Device> _device;
		std::unique_ptr<PhysicalDevice> _physicalDevice;
		std::unique_ptr<SwapChain> _swapChain;
		std::unique_ptr<CommandQueue> _graphicQueue;
		std::unique_ptr<CommandQueue> _presentQueue;
		std::unique_ptr<Surface> _surface;
	};
}
