#include "VulkanShaderModule.h"

VulkanShaderModule::VulkanShaderModule(const std::vector<char>& code, VulkanLogicalDevice* logicalDevice) : logicalDevice(logicalDevice)
{
    VkShaderModuleCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = code.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

    VkResult result = vkCreateShaderModule(logicalDevice->device, &createInfo, nullptr, &shaderModule);
    if (result != VK_SUCCESS)
    {
        throw std::runtime_error("[Ichor] Failed to create shader module");
    }
}

VulkanShaderModule::~VulkanShaderModule()
{
    vkDestroyShaderModule(logicalDevice->device, shaderModule, nullptr);
}
