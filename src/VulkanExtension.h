#ifndef VULKAN_EXTENSION_H
#define VULKAN_EXTENSION_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>

class VulkanExtension
{
public:
    static const std::vector<const char*> deviceExtensions;

    static std::vector<const char*> getRequiredExtensions();
    static bool checkDeviceExtensionSupport(VkPhysicalDevice device);
};

#endif
