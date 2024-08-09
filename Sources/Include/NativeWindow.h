#pragma once

#include<vector>

#include"InterfaceType.h"

namespace DRHI
{
	class NativeWindow
	{
	private:
		GLFWwindow* _runtimeWindow;
		std::vector<const char*> _extensions;

	public:
		void initialize(const char* title, int width, int height);

	public:
		NativeWindow()
		{
			_runtimeWindow = nullptr;
		}

		GLFWwindow* getNativeWindow()
		{
			return _runtimeWindow;
		}

		std::vector<const char*> getNativeWindowExtensions()
		{
			return _extensions;
		}
	};
}