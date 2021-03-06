#ifndef SWAPCHAIN_H
#define SWAPCHAIN_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>

#include "VulkanPhysicalDevice.h"
#include "VulkanLogicalDevice.h"
#include "VulkanSurface.h"
#include "Window.h"

class VulkanRenderPass;

struct SwapChainSupportDetails
{
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};

class VulkanSwapchain
{
private:
    VulkanLogicalDevice* logicalDevice;

    std::vector<VkImage> swapChainImages;

public:
    static SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);

    VkSwapchainKHR swapChain;

    VkFormat swapChainImageFormat;
    VkExtent2D swapChainExtent;
    std::vector<VkImageView> swapChainImageViews;
    std::vector<VkFramebuffer> swapChainFramebuffers;

    VulkanSwapchain(VulkanSurface* surface, Window* window, VulkanLogicalDevice* logicalDevice, VulkanPhysicalDevice* physicalDevice);
    ~VulkanSwapchain();

    void createFramebuffers(VulkanRenderPass* renderPass);

private:
    void createImageViews();

    VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
    VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR> availablePresentModes);
    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, Window* window);
};

#endif
