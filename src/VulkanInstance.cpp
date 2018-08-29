#include "VulkanInstance.h"

#include "VulkanExtension.h"
#include "VulkanValidationLayers.h"
#include "IchorApplication.h"

VulkanInstance::VulkanInstance()
{
    if (!VulkanValidationLayers::checkValidationLayerSupport())
    {
        throw std::runtime_error("[Ichor] Cannot enable error checking");
    }

    VkApplicationInfo appInfo = {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Ichor";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "Project Ichor";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;

    auto extensions = VulkanExtension::getRequiredExtensions();
    createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    createInfo.ppEnabledExtensionNames = extensions.data();

    if (IchorApplication::checkErrors)
    {
        createInfo.enabledLayerCount = static_cast<uint32_t>(VulkanValidationLayers::validationLayers.size());
        createInfo.ppEnabledLayerNames = VulkanValidationLayers::validationLayers.data();
    }
    else
    {
        createInfo.enabledLayerCount = 0;
    }

    VkResult result = vkCreateInstance(&createInfo, nullptr, &instance);
    if (result != VK_SUCCESS)
    {
        throw std::runtime_error("[Ichor] Failed to create vulkan instance");
    }
}

VulkanInstance::~VulkanInstance()
{
    vkDestroyInstance(instance, nullptr);
}
