#include "VulkanCommandPool.h"

#include <stdexcept>

#include "VulkanQueueFamilies.h"

VulkanCommandPool::VulkanCommandPool(VulkanSurface* surface, VulkanLogicalDevice* logicalDevice, VulkanPhysicalDevice* physicalDevice) : logicalDevice(logicalDevice)
{
    QueueFamilyIndices queueFamilyIndices = VulkanQueueFamilies::findQueueFamilies(physicalDevice->physicalDevice, surface->surface);

    VkCommandPoolCreateInfo poolInfo = {};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily;
    poolInfo.flags = 0;

    VkResult result = vkCreateCommandPool(logicalDevice->device, &poolInfo, nullptr, &commandPool);
    if (result != VK_SUCCESS)
    {
        throw std::runtime_error("[Ichor] Failed to create command pool");
    }
}

VulkanCommandPool::~VulkanCommandPool()
{
    vkDestroyCommandPool(logicalDevice->device, commandPool, nullptr);
}
