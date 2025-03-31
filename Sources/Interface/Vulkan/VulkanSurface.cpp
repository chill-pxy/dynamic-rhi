#include<stdexcept>

#include"../../Include/Vulkan/VulkanSurface.h"

namespace drhi
{
	void createSurface(VkSurfaceKHR* surface, VkInstance* instance, PlatformInfo platfromInfo)
	{
		VkWin32SurfaceCreateInfoKHR create_info = {};
		create_info.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
		create_info.hwnd = platfromInfo.window;

		PFN_vkCreateWin32SurfaceKHR pfnCreateWin32SurfaceKHR;
		pfnCreateWin32SurfaceKHR = (PFN_vkCreateWin32SurfaceKHR)vkGetInstanceProcAddr(*instance, "vkCreateWin32SurfaceKHR");

		if (pfnCreateWin32SurfaceKHR(*instance, &create_info, nullptr, surface) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create surface");
		}
		
	}
}