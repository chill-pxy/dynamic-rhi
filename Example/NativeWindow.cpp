#include"NativeWindow.h"

namespace FOCUS
{
	LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		if (uMsg == WM_CLOSE)
		{
			DestroyWindow(hWnd);
		}
		if (uMsg == WM_DESTROY)
		{
			PostQuitMessage(0);
		}
		if (uMsg == WM_SIZE)
		{
			UINT width = LOWORD(lParam);
			UINT height = HIWORD(lParam);
		}
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

	void NativeWindow::createWin32Windwow()
	{
		WNDCLASSEX wndclass = {};
		wndclass.cbSize = sizeof(wndclass);
		wndclass.hInstance = GetModuleHandleA(NULL);
		wndclass.lpszClassName = _createInfo.title;
		wndclass.lpfnWndProc = WndProc;
		RegisterClassEx(&wndclass);

		RECT winRect;
		winRect.left = 0;
		winRect.right = _createInfo.width;
		winRect.top = 0;
		winRect.bottom = _createInfo.height;

		AdjustWindowRectEx(&winRect, WS_OVERLAPPEDWINDOW, NULL, NULL);
		
		_hwnd = CreateWindowEx(
			WS_EX_ACCEPTFILES, 
			_createInfo.title,
			_createInfo.title,
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			winRect.right - winRect.left,
			winRect.bottom - winRect.top,
			NULL,
			NULL, 
			GetModuleHandleA(NULL),
			NULL
		);

		ShowWindow(_hwnd, SW_SHOW);
	}

	void NativeWindow::initialize()
	{
		createWin32Windwow();
	}

	bool NativeWindow::tick()
	{
		if (PeekMessageW(&_msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&_msg);
			DispatchMessageW(&_msg);
			if (_msg.message == WM_QUIT)
				return false;
		}

		return true;
	}
}