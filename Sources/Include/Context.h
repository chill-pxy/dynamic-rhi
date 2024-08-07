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

namespace DRHI
{
	class Context
	{
	public:
		Context()
		{
			//Ĭ��ʹ��Vulkan
			_runtimeInterface = API::VULKAN;

			_instance = std::make_unique<Instance>();
			_physicalDevice = std::make_unique<PhysicalDevice>();
			_device = std::make_unique<Device>();
			_graphicQueue = std::make_unique<CommandQueue>();
		}

		Context(API api)
		{
			_runtimeInterface = api;

			_instance = std::make_unique<Instance>(_runtimeInterface);
			_physicalDevice = std::make_unique<PhysicalDevice>(_runtimeInterface);
			_device = std::make_unique<Device>(_runtimeInterface);
			_graphicQueue = std::make_unique<CommandQueue>(_runtimeInterface);
		}

		void initialize()
		{
			_instance->createInstance();
			_physicalDevice->pickPhysicalDevice(0, _instance.get());
			_physicalDevice->pickGraphicQueueFamily();
			_device->createLogicalDevice(_physicalDevice.get(), _graphicQueue.get());
		}

	private:
		API _runtimeInterface;

		std::unique_ptr<Instance> _instance;
		std::unique_ptr<Device> _device;
		std::unique_ptr<PhysicalDevice> _physicalDevice;
		std::unique_ptr<SwapChain> _swapChain;
		std::unique_ptr<CommandQueue> _graphicQueue;
	};
}
