#ifndef VULKAN_PHYSICAL_DEVICE_H
#define VULKAN_PHYSICAL_DEVICE_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

class VulkanPhysicalDevice
{
public:
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;

    VulkanPhysicalDevice(VkInstance instance, VkSurfaceKHR surface);
    ~VulkanPhysicalDevice();

private:
    bool isDeviceSuitable(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);
};

#endif
