#ifndef VULKAN_COMMAND_POOL_H
#define VULKAN_COMMAND_POOL_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "VulkanPhysicalDevice.h"
#include "VulkanLogicalDevice.h"
#include "VulkanSurface.h"

class VulkanCommandPool
{
private:
    VulkanLogicalDevice* logicalDevice;
public:
    VkCommandPool commandPool;

    VulkanCommandPool(VulkanSurface* surface, VulkanLogicalDevice* logicalDevice, VulkanPhysicalDevice* physicalDevice);
    ~VulkanCommandPool();
};

#endif
