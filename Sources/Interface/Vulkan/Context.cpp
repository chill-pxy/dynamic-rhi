#include"../../Include/InterfaceConfig.h"

#ifdef VULKAN_IMPLEMENTATION

#include<array>

#include"../../Include/Context.h"
#include"../../Include/RenderPass.h"

namespace DRHI
{
	void Context::createRenderPass(RenderPass* prenderPass)
	{
		prenderPass->createRenderPass(_swapChain.get(), _device.get(), _physicalDevice.get());
	}
	                                                                                                                                                                                                    
	void Context::createGraphicsPipeline()
	{

	}
}

#endif