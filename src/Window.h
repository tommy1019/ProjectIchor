#ifndef GLFW_WINDOW_H
#define GLFW_WINDOW_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

class IchorApplication;

class Window
{
public:
    const int WIDTH = 800;
    const int HEIGHT = 600;

    GLFWwindow* window;

    Window(IchorApplication* app);
    ~Window();

    bool shouldClose();

    static void framebufferResizeCallback(GLFWwindow* window, int width, int height);
};

#endif
