#ifndef VULKAN_PHYSICAL_DEVICE_H
#define VULKAN_PHYSICAL_DEVICE_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "VulkanInstance.h"
#include "VulkanSurface.h"

class VulkanPhysicalDevice
{
public:
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;

    VulkanPhysicalDevice(VulkanInstance* instance, VulkanSurface* surface);

private:
    static bool isDeviceSuitable(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);
};

#endif
