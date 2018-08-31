#ifndef VULKAN_SURFACE
#define VULKAN_SURFACE

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "VulkanInstance.h"
#include "Window.h"

class VulkanSurface
{
public:
    VulkanInstance* instance;

    VkSurfaceKHR surface;

    VulkanSurface(VulkanInstance* instance, Window* window);
    ~VulkanSurface();
};

#endif
