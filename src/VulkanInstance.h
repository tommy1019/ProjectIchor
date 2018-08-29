#ifndef VULKAN_INSTANCE_H
#define VULKAN_INSTANCE_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

class VulkanInstance
{
public:
    VkInstance instance;

    VulkanInstance();
    ~VulkanInstance();
};

#endif
