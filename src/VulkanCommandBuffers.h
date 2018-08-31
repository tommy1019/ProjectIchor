#ifndef VULKAN_COMMAND_BUFFERS_H
#define VULKAN_COMMAND_BUFFERS_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>

#include "VulkanSwapchain.h"
#include "VulkanCommandPool.h"
#include "VulkanRenderPass.h"
#include "VulkanGraphicsPipeline.h"
#include "VulkanBuffer.h"

class VulkanCommandBuffers
{
private:
    VulkanLogicalDevice* logicalDevice;
    VulkanCommandPool* commandPool;
public:
    std::vector<VkCommandBuffer> commandBuffers;

    VulkanCommandBuffers(uint32_t indexCount, VulkanBuffer* vertexBuffer, VulkanBuffer* indexBuffer, VulkanCommandPool* commandPool, VulkanRenderPass* renderPass, VulkanGraphicsPipeline* graphicsPipeline, VulkanSwapchain* swapchain, VulkanLogicalDevice* logicalDevice);
    ~VulkanCommandBuffers();
};

#endif
