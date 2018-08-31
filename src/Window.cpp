#include "Window.h"

#include "IchorApplication.h"

Window::Window(IchorApplication* app)
{
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    window = glfwCreateWindow(WIDTH, HEIGHT, "Project Ichor", nullptr, nullptr);
    glfwSetWindowUserPointer(window, app);
    glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
}

Window::~Window()
{
    glfwDestroyWindow(window);
}

bool Window::shouldClose()
{
    return glfwWindowShouldClose(window);
}

void Window::framebufferResizeCallback(GLFWwindow* window, int width, int height)
{
    auto app = reinterpret_cast<IchorApplication*>(glfwGetWindowUserPointer(window));
    app->framebufferResized = true;
}
