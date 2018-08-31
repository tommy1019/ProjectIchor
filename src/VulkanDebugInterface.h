#ifndef VULKAN_DEBUG_INTERFACE_H
#define VULKAN_DEBUG_INTERFACE_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "VulkanInstance.h"

class VulkanDebugInterface
{
public:

    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);

    VkDebugUtilsMessengerEXT callback;

    VulkanInstance* instance;

    VulkanDebugInterface(VulkanInstance* instance);
    ~VulkanDebugInterface();
};

#endif
