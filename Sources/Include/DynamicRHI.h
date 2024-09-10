#pragma once
#include"InterfaceType.h"

namespace DRHI
{
	class DynamicRHI
	{
	public:
		//initialize vulkan rhi member
		virtual void initialize() = 0;
		//clean vulkan rhi member
		virtual void clean() = 0;
		//call vkCommandBegin function
		virtual void prepareCommandBuffer(DynamicBuffer* vertexBuffer, DynamicBuffer* indexBuffer) = 0;
		//call within render loop
		virtual void frameOnTick() = 0;
		//Buffer class
		virtual void createDynamicBuffer(DynamicBuffer* vertexBuffer, DynamicDeviceMemory* deviceMemory, uint64_t bufferSize, void* bufferData) = 0;
	};
}