#include "IchorApplication.h"

#include <set>
#include <fstream>

bool IchorApplication::checkErrors = true;

void IchorApplication::run()
{
    std::cout << "[Ichor] Starting up..." << std::endl;

    glfwInit();
    window = new Window(this);
    initVulkan();
    
    gameLoop();

    cleanup();
}

void IchorApplication::cleanup()
{
    delete commandBuffers;
    delete graphicsPipeline;
    delete renderPass;
    delete swapchain;
    delete indexBuffer;
    delete vertexBuffer;

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
    {
        vkDestroySemaphore(logicalDevice->device, renderFinishedSemaphores[i], nullptr);
        vkDestroySemaphore(logicalDevice->device, imageAvailableSemaphores[i], nullptr);
        vkDestroyFence(logicalDevice->device, inFlightFences[i], nullptr);
    }

    delete commandPool;

    delete logicalDevice;

    if (checkErrors)
        delete debugInterface;

    delete surface;
    delete instance;

    delete window;

    glfwTerminate();
}

void IchorApplication::gameLoop()
{
    std::cout << "[Ichor] Entering game loop" << std::endl;

    while (!window->shouldClose())
    {
        glfwPollEvents();
        drawFrame();
    }

    vkDeviceWaitIdle(logicalDevice->device);
}

void IchorApplication::drawFrame()
{
    vkWaitForFences(logicalDevice->device, 1, &inFlightFences[currentFrame], VK_TRUE, std::numeric_limits<uint64_t>::max());

    uint32_t imageIndex;
    VkResult result = vkAcquireNextImageKHR(logicalDevice->device, swapchain->swapChain, std::numeric_limits<uint64_t>::max(), imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex);

    if (result == VK_ERROR_OUT_OF_DATE_KHR)
    {
        recreateSwapChain();
        return;
    }
    else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
    {
        throw std::runtime_error("[Ichor] Failed to acquire swap chain image");
    }

    VkSubmitInfo submitInfo = {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore waitSemaphores[] = {imageAvailableSemaphores[currentFrame]};
    VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;

    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &(commandBuffers->commandBuffers[imageIndex]);

    VkSemaphore signalSemaphores[] = {renderFinishedSemaphores[currentFrame]};
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    vkResetFences(logicalDevice->device, 1, &inFlightFences[currentFrame]);

    VkResult sumbitResult = vkQueueSubmit(logicalDevice->graphicsQueue, 1, &submitInfo, inFlightFences[currentFrame]);
    if (sumbitResult != VK_SUCCESS)
    {
        throw std::runtime_error("[Ichor] Failed to submit draw command buffer");
    }

    VkPresentInfoKHR presentInfo = {};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;

    VkSwapchainKHR swapChains[] = {swapchain->swapChain};
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;
    presentInfo.pImageIndices = &imageIndex;

    presentInfo.pResults = nullptr;

    result = vkQueuePresentKHR(logicalDevice->presentQueue, &presentInfo);

    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR  || framebufferResized)
    {
        framebufferResized = false;
        recreateSwapChain();
    }
    else if (result != VK_SUCCESS)
    {
        throw std::runtime_error("[Ichor] Failed to present swap chain image");
    }

    currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}

void IchorApplication::initVulkan()
{
    instance = new VulkanInstance();

    if (checkErrors)
        debugInterface = new VulkanDebugInterface(instance);
    
    surface = new VulkanSurface(instance, window);

    physicalDevice = new VulkanPhysicalDevice(instance, surface);
    logicalDevice = new VulkanLogicalDevice(physicalDevice, surface);

    swapchain = new VulkanSwapchain(surface, window, logicalDevice, physicalDevice);
    renderPass = new VulkanRenderPass(swapchain, logicalDevice);
    graphicsPipeline = new VulkanGraphicsPipeline(renderPass, swapchain, logicalDevice);

    swapchain->createFramebuffers(renderPass);

    commandPool = new VulkanCommandPool(surface, logicalDevice, physicalDevice);

    vertexBuffer = new VulkanBuffer(sizeof(vertices[0]) * vertices.size(), (void*)vertices.data(), VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, this);
    indexBuffer = new VulkanBuffer(sizeof(indices[0]) * indices.size(), (void*)indices.data(), VK_BUFFER_USAGE_INDEX_BUFFER_BIT, this);

    commandBuffers = new VulkanCommandBuffers(static_cast<uint32_t>(indices.size()), vertexBuffer, indexBuffer, commandPool, renderPass, graphicsPipeline, swapchain, logicalDevice);

    createSyncObjects();
}

void IchorApplication::recreateSwapChain()
{
    int width = 0, height = 0;
    while (width == 0 || height == 0)
    {
        glfwGetFramebufferSize(window->window, &width, &height);
        glfwWaitEvents();
    }

    vkDeviceWaitIdle(logicalDevice->device);

    delete commandBuffers;

    delete graphicsPipeline;

    delete renderPass;
    delete swapchain;

    swapchain = new VulkanSwapchain(surface, window, logicalDevice, physicalDevice);
    renderPass = new VulkanRenderPass(swapchain, logicalDevice);
    graphicsPipeline = new VulkanGraphicsPipeline(renderPass, swapchain, logicalDevice);
    swapchain->createFramebuffers(renderPass);

    commandBuffers = new VulkanCommandBuffers(static_cast<uint32_t>(indices.size()), vertexBuffer, indexBuffer, commandPool, renderPass, graphicsPipeline, swapchain, logicalDevice);
}

void IchorApplication::createSyncObjects()
{
    imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);

    VkSemaphoreCreateInfo semaphoreInfo = {};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceInfo = {};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
    {
        VkResult imageAvailableResult = vkCreateSemaphore(logicalDevice->device, &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]);
        VkResult renderFinishedResult = vkCreateSemaphore(logicalDevice->device, &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]);
        VkResult fenceResult = vkCreateFence(logicalDevice->device, &fenceInfo, nullptr, &inFlightFences[i]);

        if (imageAvailableResult != VK_SUCCESS || renderFinishedResult != VK_SUCCESS || fenceResult != VK_SUCCESS)
        {
            throw std::runtime_error("[Ichor] Failed to create sync objects");
        }
    }
}
