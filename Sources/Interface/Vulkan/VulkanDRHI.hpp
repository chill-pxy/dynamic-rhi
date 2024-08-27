#pragma once

#include "../../Include/DynamicRHI.hpp"

namespace DRHI
{
	class VulkanDRHI : public DynamicRHI
	{
	private:


	public:
		virtual void initialize() final override;
	};
}