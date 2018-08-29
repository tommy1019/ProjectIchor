#ifndef VULKAN_H
#define VULKAN_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

class Vulkan
{
private:
    VkInstance instance;
public:
    Vulkan();
    ~Vulkan();
};

#endif
