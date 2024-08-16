#pragma once

#include<variant>

#include"InterfaceType.h"

namespace DRHI
{
	class ShaderModule
	{
	private:
		std::variant<VkShaderModule*> _runtimeShaderModule;

	public:
		ShaderModule()
		{
			_runtimeShaderModule = new VkShaderModule();
		}

		ShaderModule(API api)
		{
			switch (api)
			{
			case VULKAN:
				_runtimeShaderModule = new VkShaderModule();
				break;
			case DIRECT3D12:
				break;
			default:
				break;
			}
		}

		VkShaderModule* getVkShaderModule()
		{
			if (std::holds_alternative<VkShaderModule*>(_runtimeShaderModule))
			{
				return std::get<VkShaderModule*>(_runtimeShaderModule);
			}
			else
			{
				//error
				return nullptr;
			}
		}
	};
}