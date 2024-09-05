#include<stdexcept>

#include"../../Include/Vulkan/VulkanSurface.h"

namespace DRHI
{
	void createSurface(VkSurfaceKHR* surface, VkInstance* instance, PlatformInfo platfromInfo)
	{
		VkWin32SurfaceCreateInfoKHR create_info = {};
		create_info.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
		create_info.hinstance = platfromInfo.instance;
		create_info.hwnd = platfromInfo.window;

		if (vkCreateWin32SurfaceKHR(instance, &create_info, get_allocation_callbacks(VK_OBJECT_TYPE_SURFACE_KHR), &surface) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create surface");
		}
		
	}
}