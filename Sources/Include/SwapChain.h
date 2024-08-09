#pragma once
#include<variant>
#include<iostream>
#include<vector>

#include"InterfaceType.h"
#include"PhysicalDevice.h"
#include"Device.h"
#include"Surface.h"
#include"ImageView.h"

namespace DRHI
{
	class SwapChain
	{
	private:
		std::variant<VkSwapchainKHR*>            _runtimeSwapChain;
		std::variant<VkFormat>                   _swapChainImageFormat;
		std::variant<VkExtent2D>                 _swapChainExtent;

		std::variant<std::vector<VkImage>>       _swapChainImages;
		std::variant<std::vector<VkImageView>>   _swapChainImageViews;
		std::variant<std::vector<VkFramebuffer>> _swapChainFramebuffers;

	public:
		void createSwapChain(PhysicalDevice* phyDevice, Device* device, Surface* surface, GLFWwindow* window);
		void createImageViews(Device* device);

	public:

		SwapChain()
		{
			_runtimeSwapChain = new VkSwapchainKHR();
			_swapChainImageFormat = VkFormat();
			_swapChainExtent = VkExtent2D();

			_swapChainImages = std::vector<VkImage>();
			_swapChainImageViews = std::vector<VkImageView>();
			_swapChainFramebuffers = std::vector<VkFramebuffer>();
		}

		SwapChain(API api)
		{
			switch (api)
			{
			case API::VULKAN:
				_runtimeSwapChain = new VkSwapchainKHR();
				_swapChainImageFormat = VkFormat();
				_swapChainExtent = VkExtent2D();

				_swapChainImages = std::vector<VkImage>();
				_swapChainImageViews = std::vector<VkImageView>();
				_swapChainFramebuffers = std::vector<VkFramebuffer>();
				break;
			}
		}

		VkSwapchainKHR* getVkSwapChain()
		{
			if (std::holds_alternative<VkSwapchainKHR*>(_runtimeSwapChain))
			{
				return std::get<VkSwapchainKHR*>(_runtimeSwapChain);
			}
			else
			{
				std::cout << "none vk swapchain";
				return nullptr;
			}
		}

		VkFormat getVkSwapChainImageFormat()
		{
			if (std::holds_alternative<VkFormat>(_swapChainImageFormat))
			{
				return std::get<VkFormat>(_swapChainImageFormat);
			}
			else
			{
				std::cout << "none vk swapchain image format";
				//return nullptr;
			}
		}

		VkExtent2D getVkSwapChainExtent()
		{
			if (std::holds_alternative<VkExtent2D>(_swapChainExtent))
			{
				return std::get<VkExtent2D>(_swapChainExtent);
			}
			else
			{
				std::cout << "none vk swapchain extent";
				//return nullptr;
			}
		}

		std::vector<VkImage> getVkSwapChainImages()
		{
			if (std::holds_alternative<std::vector<VkImage>>(_swapChainImages))
			{
				return std::get<std::vector<VkImage>>(_swapChainImages);
			}
			else
			{
				std::cout << "none vk swapchain images";
				//return nullptr;
			}
		}

		std::vector<VkImageView> getVkSwapChainImageViews()
		{
			if (std::holds_alternative<std::vector<VkImageView>>(_swapChainImageViews))
			{
				return std::get<std::vector<VkImageView>>(_swapChainImageViews);
			}
			else
			{
				std::cout << "none vk swapchain views";
				//return nullptr;
			}
		}

		std::vector<VkFramebuffer> getVkSwapChainFramebuffers()
		{
			if (std::holds_alternative<std::vector<VkFramebuffer>>(_swapChainFramebuffers))
			{
				return std::get<std::vector<VkFramebuffer>>(_swapChainFramebuffers);
			}
			else
			{
				std::cout << "none vk swapchain frambuffers";
				//return nullptr;
			}
		}
	};
}