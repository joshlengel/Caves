#pragma once

#include<cstdint>

struct GLFWwindow;

class Window
{
public:
    Window(uint32_t width, uint32_t height, const char *title);
    ~Window();

    uint32_t GetWidth() const;
    uint32_t GetHeight() const;

    void Show();
    bool ShouldClose();
    void Update();

private:
    GLFWwindow *m_handle;
    uint32_t m_width, m_height;
};