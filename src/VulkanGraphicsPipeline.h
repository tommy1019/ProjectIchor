#ifndef VULKAN_GRAPHICS_PIPELINE_H
#define VULKAN_GRAPHICS_PIPELINE_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "VulkanSwapchain.h"
#include "VulkanLogicalDevice.h"
#include "VulkanRenderPass.h"

class VulkanGraphicsPipeline
{
private:
    VulkanLogicalDevice* logicalDevice;
public:
    VkPipelineLayout pipelineLayout;
    VkPipeline graphicsPipeline;

    VulkanGraphicsPipeline(VulkanRenderPass* renderPass, VulkanSwapchain* swapchain, VulkanLogicalDevice* logicalDevice);
    ~VulkanGraphicsPipeline();
};

#endif
