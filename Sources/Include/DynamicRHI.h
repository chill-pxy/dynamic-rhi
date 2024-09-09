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
		virtual void prepareCommandBuffer() = 0;
		//call within render loop
		virtual void frameOnTick() = 0;



		//Buffer class
		virtual void iCreateDynamicBuffer(DynamicBuffer* vertexBuffer, DynamicDeviceMemory* deviceMemory, uint64_t bufferSize, void* bufferData) = 0;
		//
		virtual void iBindVertexBuffer() = 0;
		//
		virtual void iBindIndexBuffer() = 0;
	};
}