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
	};
}