#pragma once

#include<Windows.h>
#include<vector>

namespace FOCUS
{
    struct NativeWindowCreateInfo
    {
        const char* title;
        uint32_t width;
        uint32_t height;
    };

    class NativeWindow
    {
    public:
        NativeWindowCreateInfo _createInfo{};
        HWND _hwnd{NULL};
        MSG _msg{NULL};

    public:
        NativeWindow() = delete;
        NativeWindow(NativeWindowCreateInfo info) : _createInfo(info) {}

        void initialize();
        bool tick();

    private:
        void createWin32Windwow();
    };
}
