#ifndef ICHOR_APPLICATION_H
#define ICHOR_APPLICATION_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include <iostream>
#include <vector>
#include <array>

#include "Window.h"

#include "VulkanInstance.h"
#include "VulkanSurface.h"
#include "VulkanPhysicalDevice.h"
#include "VulkanLogicalDevice.h"
#include "VulkanQueueFamilies.h"
#include "VulkanBuffer.h"
#include "VulkanDebugInterface.h"
#include "VulkanRenderPass.h"
#include "VulkanSwapchain.h"
#include "VulkanGraphicsPipeline.h"
#include "VulkanCommandPool.h"
#include "VulkanCommandBuffers.h"

#include "Vertex.h"

class IchorApplication
{
public:
    const uint32_t WIDTH = 800;
    const uint32_t HEIGHT = 600;

    const int MAX_FRAMES_IN_FLIGHT = 2;

    const std::vector<Vertex> vertices =
    {
        {{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
        {{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
        {{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
        {{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}}
    };

    const std::vector<uint16_t> indices =
    {
        0, 1, 2, 2, 3, 0
    };

    Window* window;
    VulkanInstance* instance;
    VulkanSurface* surface;

    VulkanDebugInterface* debugInterface;

    VulkanPhysicalDevice* physicalDevice;
    VulkanLogicalDevice* logicalDevice;

    VulkanSwapchain* swapchain;

    VulkanGraphicsPipeline* graphicsPipeline;

    VulkanBuffer* indexBuffer;
    VulkanBuffer* vertexBuffer;

    VulkanRenderPass* renderPass;

    VulkanCommandPool* commandPool;
    VulkanCommandBuffers* commandBuffers;

    std::vector<VkSemaphore> imageAvailableSemaphores;
    std::vector<VkSemaphore> renderFinishedSemaphores;

    std::vector<VkFence> inFlightFences;

    size_t currentFrame = 0;

    bool framebufferResized = false;

    void createWindow();
    static void framebufferResizeCallback(GLFWwindow* window, int width, int height);

    void initVulkan();

    void recreateSwapChain();

    void createSyncObjects();

    void gameLoop();
    void drawFrame();

    void cleanup();

public:
    static bool checkErrors;

    void run();
};

#endif
