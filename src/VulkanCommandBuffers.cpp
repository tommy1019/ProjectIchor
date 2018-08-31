#include "VulkanCommandBuffers.h"

VulkanCommandBuffers::VulkanCommandBuffers(uint32_t indexCount, VulkanBuffer* vertexBuffer, VulkanBuffer* indexBuffer, VulkanCommandPool* commandPool, VulkanRenderPass* renderPass, VulkanGraphicsPipeline* graphicsPipeline, VulkanSwapchain* swapchain, VulkanLogicalDevice* logicalDevice) : logicalDevice(logicalDevice), commandPool(commandPool)
{
    commandBuffers.resize(swapchain->swapChainFramebuffers.size());

    VkCommandBufferAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = commandPool->commandPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = (uint32_t) commandBuffers.size();

    VkResult result = vkAllocateCommandBuffers(logicalDevice->device, &allocInfo, commandBuffers.data());
    if (result != VK_SUCCESS)
    {
        throw std::runtime_error("[Ichor] Failed to allocate command buffers");
    }

    for (size_t i = 0; i < commandBuffers.size(); i++)
    {
        VkCommandBufferBeginInfo beginInfo = {};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
        beginInfo.pInheritanceInfo = nullptr;

        VkResult result = vkBeginCommandBuffer(commandBuffers[i], &beginInfo);
        if (result != VK_SUCCESS)
        {
            throw std::runtime_error("[Ichor] Failed to begin recording command buffer");
        }

        VkRenderPassBeginInfo renderPassInfo = {};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = renderPass->renderPass;
        renderPassInfo.framebuffer = swapchain->swapChainFramebuffers[i];

        renderPassInfo.renderArea.offset = {0, 0};
        renderPassInfo.renderArea.extent = swapchain->swapChainExtent;

        VkClearValue clearColor;
        clearColor.color = {{0.0f, 0.0f, 0.0f, 1.0f}};
        renderPassInfo.clearValueCount = 1;
        renderPassInfo.pClearValues = &clearColor;

        vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
        {
            vkCmdBindPipeline(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline->graphicsPipeline);

            VkBuffer vertexBuffers[] = {vertexBuffer->buffer};
            VkDeviceSize offsets[] = {0};

            vkCmdBindVertexBuffers(commandBuffers[i], 0, 1, vertexBuffers, offsets);
            vkCmdBindIndexBuffer(commandBuffers[i], indexBuffer->buffer, 0, VK_INDEX_TYPE_UINT16);

            vkCmdDrawIndexed(commandBuffers[i], indexCount, 1, 0, 0, 0);
        }
        vkCmdEndRenderPass(commandBuffers[i]);

        VkResult recordResult = vkEndCommandBuffer(commandBuffers[i]);
        if (recordResult != VK_SUCCESS)
        {
            throw std::runtime_error("[Ichor] Failed to record command buffer");
        }
    }
}

VulkanCommandBuffers::~VulkanCommandBuffers()
{
    vkFreeCommandBuffers(logicalDevice->device, commandPool->commandPool, static_cast<uint32_t>(commandBuffers.size()), commandBuffers.data());
}
