#ifndef VULKAN_RENDER_PASS_H
#define VULKAN_RENDER_PASS_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "VulkanSwapchain.h"
#include "VulkanLogicalDevice.h"

class VulkanRenderPass
{
private:
    VulkanLogicalDevice* logicalDevice;
public:
    VkRenderPass renderPass;

    VulkanRenderPass(VulkanSwapchain* swapchain, VulkanLogicalDevice* logicalDevice);
    ~VulkanRenderPass();
};

#endif
