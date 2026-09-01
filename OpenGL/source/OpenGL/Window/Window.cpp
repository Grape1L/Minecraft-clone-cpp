#include <OpenGL/Window/Window.h>
#include <windows.h>
#include <assert.h>
#include <glad/glad.h>
#include <glad/glad_wgl.h>

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    Window* window = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);

    switch (msg) {
        case WM_SETFOCUS:
            if (window) window->window_focused = true;
            break;

        case WM_KILLFOCUS:
            if (window) window->window_focused = false;
            break;

        case WM_CLOSE: {
            PostQuitMessage(0);
            break;
        }

        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

Window::Window() {
    ShowCursor(false);

    // fullScreen();



    WNDCLASSEX wc = {};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.lpszClassName = (LPCWSTR)"OpenGLWindow";
    wc.lpfnWndProc = &WndProc;

    auto classID = RegisterClassEx(&wc);
    assert(classID);

    m_handle = CreateWindowEx(0, MAKEINTATOM(classID), (LPCWSTR)"SomethingIDK", WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU, CW_USEDEFAULT, 
        CW_USEDEFAULT, 1920, 1080, NULL, NULL, GetModuleHandle(NULL), NULL);

    assert(m_handle);

    SetWindowLongPtr((HWND)m_handle, GWLP_USERDATA, (LONG_PTR)this);

    ShowWindow((HWND)m_handle, SW_SHOW);
    UpdateWindow((HWND)m_handle);

    // OpenGL Render Context

    auto hDC = GetDC(HWND(m_handle));
    m_hdc = hDC;

    int pixelFormatAttributes[] = {
        WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
        WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
        WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
        WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
        WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
        WGL_COLOR_BITS_ARB, 32,
        WGL_DEPTH_BITS_ARB, 24,
        WGL_STENCIL_BITS_ARB, 8,
        0
    };

    int pixelFormat = 0;
    UINT numFormats = 0;
    wglChoosePixelFormatARB(hDC, pixelFormatAttributes, nullptr, 1, &pixelFormat, &numFormats);
    assert(numFormats);

    PIXELFORMATDESCRIPTOR pixelFormatDesc = {};
    DescribePixelFormat(hDC, pixelFormat, sizeof(PIXELFORMATDESCRIPTOR), &pixelFormatDesc);
    SetPixelFormat(hDC, pixelFormat, &pixelFormatDesc);

    int openGLAttributes[] = {
        WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
        WGL_CONTEXT_MINOR_VERSION_ARB, 6,
        WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
        0
    };

    m_context = wglCreateContextAttribsARB(hDC, 0, openGLAttributes);
    assert(m_context);
}

Window::~Window() {
    wglDeleteContext((HGLRC)m_context);
    DestroyWindow((HWND)m_handle);
    ChangeDisplaySettings(NULL, 0);
}

void Window::makeCurrentContext() {
    wglMakeCurrent(GetDC(HWND(m_handle)), HGLRC(m_context));
}

void Window::present(bool vsync) {
    wglSwapIntervalEXT(vsync);
    // wglSwapLayerBuffers(GetDC(HWND(m_handle)), WGL_SWAP_MAIN_PLANE);
    SwapBuffers((HDC)m_hdc);
}

Rect Window::getInnerSize() {
    RECT rect = {};
    GetClientRect((HWND)m_handle, &rect);
    return Rect(rect.right - rect.left, rect.bottom - rect.top);
}

void Window::windowedScreen() {
    
}

void Window::fullScreen() {
    int screenWidth = GetSystemMetrics(SM_CXSCREEN); int screenHeight = GetSystemMetrics(SM_CYSCREEN);


}