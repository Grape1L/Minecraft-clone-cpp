#pragma once

#include <OpenGL/Math/Rect.h>

class Window {
public:
    Window();
    ~Window();

    Rect getInnerSize();

    void makeCurrentContext();
    void present(bool vsync);
private:
    void* m_handle = nullptr;
    void* m_context = nullptr;

    void* m_hdc = nullptr;

private:
    void fullScreen();
    void windowedScreen();

public:
    bool isFocused() const { return window_focused; }

public:
    bool window_focused = true;
};