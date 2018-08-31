#include "VulkanSurface.h"

#include <stdexcept>

VulkanSurface::VulkanSurface(VulkanInstance* instance, Window* window) : instance(instance)
{
    VkResult result = glfwCreateWindowSurface(instance->instance, window->window, nullptr, &surface);
    if (result != VK_SUCCESS)
    {
        throw std::runtime_error("[Ichor] Failed to create window surface");
    }
}

VulkanSurface::~VulkanSurface()
{
    vkDestroySurfaceKHR(instance->instance, surface, nullptr);
}
