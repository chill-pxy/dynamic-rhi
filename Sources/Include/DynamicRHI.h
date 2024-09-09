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
		//
		virtual void bindVertexBuffer() = 0;
		//
		virtual void bindIndexBuffer() = 0;
	};
}