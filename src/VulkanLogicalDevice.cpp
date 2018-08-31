#include "VulkanLogicalDevice.h"

#include <vector>
#include <set>

#include "IchorApplication.h"
#include "VulkanQueueFamilies.h"
#include "VulkanExtension.h"
#include "VulkanValidationLayers.h"

VulkanLogicalDevice::VulkanLogicalDevice(VulkanPhysicalDevice* physicalDevice, VulkanSurface* surface)
{
    QueueFamilyIndices indices = VulkanQueueFamilies::findQueueFamilies(physicalDevice->physicalDevice, surface->surface);

    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    std::set<int> uniqueQueueFamilies = {indices.graphicsFamily, indices.presentFamily};

    float queuePriority = 1.0f;
    for (int queueFamily : uniqueQueueFamilies)
    {
        VkDeviceQueueCreateInfo queueCreateInfo = {};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = queueFamily;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;
        queueCreateInfos.push_back(queueCreateInfo);
    }

    VkPhysicalDeviceFeatures deviceFeatures = {};

    VkDeviceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

    createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
    createInfo.pQueueCreateInfos = queueCreateInfos.data();

    createInfo.pEnabledFeatures = &deviceFeatures;

    createInfo.enabledExtensionCount = static_cast<uint32_t>(VulkanExtension::deviceExtensions.size());
    createInfo.ppEnabledExtensionNames = VulkanExtension::deviceExtensions.data();

    if (IchorApplication::checkErrors)
    {
        createInfo.enabledLayerCount = static_cast<uint32_t>(VulkanValidationLayers::validationLayers.size());
        createInfo.ppEnabledLayerNames = VulkanValidationLayers::validationLayers.data();
    }
    else
    {
        createInfo.enabledLayerCount = 0;
    }

    VkResult result = vkCreateDevice(physicalDevice->physicalDevice, &createInfo, nullptr, &device);
    if (result != VK_SUCCESS)
    {
        throw std::runtime_error("[Ichor] Failed to create logical device");
    }

    vkGetDeviceQueue(device, indices.graphicsFamily, 0, &graphicsQueue);
    vkGetDeviceQueue(device, indices.presentFamily, 0, &presentQueue);
}

VulkanLogicalDevice::~VulkanLogicalDevice()
{
    vkDestroyDevice(device, nullptr);
}
