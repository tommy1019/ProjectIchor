#ifndef VULKAN_SHADER_MODULE_H
#define VULKAN_SHADER_MODULE_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>

#include "VulkanLogicalDevice.h"

class VulkanShaderModule
{
private:
    VulkanLogicalDevice* logicalDevice;
public:
    VkShaderModule shaderModule;

    VulkanShaderModule(const std::vector<char>& code, VulkanLogicalDevice* logicalDevice);
    ~VulkanShaderModule();
};

#endif
