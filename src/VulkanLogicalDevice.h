#ifndef VULKAN_LOGICAL_DEVICE_H
#define VULKAN_LOGICAL_DEVICE_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "VulkanPhysicalDevice.h"
#include "VulkanSurface.h"

class VulkanLogicalDevice
{
public:
    VkDevice device;

    VkQueue graphicsQueue;
    VkQueue presentQueue;
    
    VulkanLogicalDevice(VulkanPhysicalDevice* physicalDevice, VulkanSurface* surface);
    ~VulkanLogicalDevice();
};

#endif
