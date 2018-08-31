#ifndef VULKAN_BUFFER_H
#define VULKAN_BUFFER_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "VulkanLogicalDevice.h"
#include "VulkanPhysicalDevice.h"
#include "VulkanCommandPool.h"

class IchorApplication;

class VulkanBuffer
{
public:
    static void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkDevice device, VkPhysicalDevice physicalDevice, VkBuffer& buffer, VkDeviceMemory& bufferMemory);

    static void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size, VulkanCommandPool* commandPool, VulkanLogicalDevice* logicalDevice);

    static uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties, VkPhysicalDevice physicalDevice);

    VkBuffer buffer;
    VkDeviceMemory bufferMemory;

    VulkanLogicalDevice* logicalDevice;

    VulkanBuffer(size_t size, void* inData, VkBufferUsageFlags usageType, IchorApplication* app);
    VulkanBuffer(size_t size, void* inData, VkBufferUsageFlags usageType, VulkanCommandPool* commandPool, VulkanLogicalDevice* logicalDevice, VulkanPhysicalDevice* physicalDevice);
    ~VulkanBuffer();
};

#endif
