#include "VulkanPhysicalDevice.h"

#include <vector>

#include "VulkanQueueFamilies.h"
#include "VulkanExtension.h"
#include "VulkanSwapchain.h"

VulkanPhysicalDevice::VulkanPhysicalDevice(VulkanInstance* instance, VulkanSurface* surface)
{
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(instance->instance, &deviceCount, nullptr);

    if (deviceCount == 0)
    {
        throw std::runtime_error("[Ichor] No GPUs detected with vulkan support");
    }

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(instance->instance, &deviceCount, devices.data());

    for (const auto& device : devices)
    {
        if (isDeviceSuitable(device, surface->surface))
        {
            physicalDevice = device;
            break;
        }
    }

    if (physicalDevice == VK_NULL_HANDLE)
    {
        throw std::runtime_error("[Ichor] Failed to find a suitable GPU");
    }
}

bool VulkanPhysicalDevice::isDeviceSuitable(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface)
{
    VkPhysicalDeviceProperties deviceProperties;
    vkGetPhysicalDeviceProperties(physicalDevice, &deviceProperties);

    VkPhysicalDeviceFeatures deviceFeatures;
    vkGetPhysicalDeviceFeatures(physicalDevice, &deviceFeatures);

    bool isDiscrete = deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU;

    QueueFamilyIndices indices = VulkanQueueFamilies::findQueueFamilies(physicalDevice, surface);

    bool extensionsSupported = VulkanExtension::checkDeviceExtensionSupport(physicalDevice);

    bool swapChainAdequate = false;
    if (extensionsSupported)
    {
        SwapChainSupportDetails swapChainSupport = VulkanSwapchain::querySwapChainSupport(physicalDevice, surface);
        swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
    }

    return indices.isComplete() && isDiscrete && extensionsSupported && swapChainAdequate;
}
