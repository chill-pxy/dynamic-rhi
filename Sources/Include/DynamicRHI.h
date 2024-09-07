#pragma once
#include"InterfaceType.h"

namespace DRHI
{
	class DynamicRHI
	{
	public:
		virtual void initialize() = 0;
		virtual void clean() = 0;
		virtual void beginCommandBuffer() = 0;
		virtual void prepareFrame() = 0;
		virtual void submitFrame() = 0;
		virtual void draw() = 0;
	};
}