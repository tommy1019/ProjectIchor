#include "VulkanBuffer.h"

#include "IchorApplication.h"

#include <stdexcept>
#include <cstring>

void VulkanBuffer::createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkDevice device, VkPhysicalDevice physicalDevice, VkBuffer& buffer, VkDeviceMemory& bufferMemory)
{
    VkBufferCreateInfo bufferInfo = {};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;
    bufferInfo.usage = usage;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    VkResult result = vkCreateBuffer(device, &bufferInfo, nullptr, &buffer);

    if (result != VK_SUCCESS)
    {
        throw std::runtime_error("[Ichor] Failed to create buffer");
    }

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(device, buffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties, physicalDevice);

    result = vkAllocateMemory(device, &allocInfo, nullptr, &bufferMemory);

    if (result != VK_SUCCESS)
    {
        throw std::runtime_error("[Ichor] Failed to allocate buffer memory");
    }

    vkBindBufferMemory(device, buffer, bufferMemory, 0);
}

void VulkanBuffer::copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size, VulkanCommandPool* commandPool, VulkanLogicalDevice* logicalDevice)
{
    VkCommandBufferAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = commandPool->commandPool;
    allocInfo.commandBufferCount = 1;

    VkCommandBuffer commandBuffer;
    vkAllocateCommandBuffers(logicalDevice->device, &allocInfo, &commandBuffer);

    VkCommandBufferBeginInfo beginInfo = {};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(commandBuffer, &beginInfo);
    {
        VkBufferCopy copyRegion = {};
        copyRegion.srcOffset = 0;
        copyRegion.dstOffset = 0;
        copyRegion.size = size;
        vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);
    }
    vkEndCommandBuffer(commandBuffer);

    VkSubmitInfo submitInfo = {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

    vkQueueSubmit(logicalDevice->graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(logicalDevice->graphicsQueue);

    vkFreeCommandBuffers(logicalDevice->device, commandPool->commandPool, 1, &commandBuffer);
}

uint32_t VulkanBuffer::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties, VkPhysicalDevice physicalDevice)
{
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
    {
        if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
        {
            return i;
        }
    }

    throw std::runtime_error("[Ichor] Failed to find suitable memory type");
}

VulkanBuffer::VulkanBuffer(size_t size, void* inData, VkBufferUsageFlags usageType, IchorApplication* app) : VulkanBuffer(size, inData, usageType, app->commandPool, app->logicalDevice, app->physicalDevice)
{
}

VulkanBuffer::VulkanBuffer(size_t size, void* inData, VkBufferUsageFlags usageType, VulkanCommandPool* commandPool, VulkanLogicalDevice* logicalDevice, VulkanPhysicalDevice* physicalDevice) : logicalDevice(logicalDevice)
{
    VkDeviceSize bufferSize = size;

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, logicalDevice->device, physicalDevice->physicalDevice, stagingBuffer, stagingBufferMemory);

    void* data;
    vkMapMemory(logicalDevice->device, stagingBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, inData, (size_t) bufferSize);
    vkUnmapMemory(logicalDevice->device, stagingBufferMemory);

    createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | usageType, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, logicalDevice->device, physicalDevice->physicalDevice, buffer, bufferMemory);

    copyBuffer(stagingBuffer, buffer, bufferSize, commandPool, logicalDevice);

    vkDestroyBuffer(logicalDevice->device, stagingBuffer, nullptr);
    vkFreeMemory(logicalDevice->device, stagingBufferMemory, nullptr);
}

VulkanBuffer::~VulkanBuffer()
{
    vkDestroyBuffer(logicalDevice->device, buffer, nullptr);
    vkFreeMemory(logicalDevice->device, bufferMemory, nullptr);
}
