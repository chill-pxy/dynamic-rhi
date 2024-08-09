#pragma once

#include<variant>
#include<iostream>

#include"InterfaceType.h"

namespace DRHI
{
	class ImageView
	{
	private:
		std::variant<VkImageView*> _runtimeImageView;

	public:
		ImageView()
		{
			_runtimeImageView = new VkImageView();
		}

		ImageView(API api)
		{
			switch (api)
			{
			case API::VULKAN:
				_runtimeImageView = new VkImageView();
				break;
			}
		}

		VkImageView* getVkImageView()
		{
			if (std::holds_alternative<VkImageView*>(_runtimeImageView))
			{
				return std::get<VkImageView*>(_runtimeImageView);
			}
			else
			{
				std::cout << "none vk image view";
				return nullptr;
			}
		}
	};
}