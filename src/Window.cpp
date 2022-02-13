#include"Window.h"

#define GLFW_INCLUDE_NONE
#include<GLFW/glfw3.h>
#include<glad/glad.h>

#include<stdexcept>

Window::Window(uint32_t width, uint32_t height, const char *title):
    m_width(width), m_height(height)
{
    // Initialize GLFW and create window
    if (!glfwInit())
        throw std::runtime_error("Error initializing GLFW");
    
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
    m_handle = glfwCreateWindow(static_cast<int>(width), static_cast<int>(height), title, nullptr, nullptr);

    // Center window
    const GLFWvidmode *screen_props = glfwGetVideoMode(glfwGetPrimaryMonitor());
    glfwSetWindowPos(m_handle, (screen_props->width - width) / 2, (screen_props->height - height) / 2);

    // Setup OpenGL
    glfwMakeContextCurrent(m_handle);
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(&glfwGetProcAddress)))
        throw std::runtime_error("Error initializing GLAD");
}

Window::~Window()
{
    glfwTerminate();
}

uint32_t Window::GetWidth() const { return m_width; }
uint32_t Window::GetHeight() const { return m_height; }

void Window::Show()
{
    glfwShowWindow(m_handle);
}

bool Window::ShouldClose()
{
    return glfwWindowShouldClose(m_handle);
}

void Window::Update()
{
    glfwPollEvents();
    glfwSwapBuffers(m_handle);
}