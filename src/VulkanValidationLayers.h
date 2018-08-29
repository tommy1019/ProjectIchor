#ifndef VULKAN_VALIDATION_LAYER_H
#define VULKAN_VALIDATION_LAYER_H

#include <vector>

class VulkanValidationLayers
{
public:
    static const std::vector<const char*> validationLayers;

    static bool checkValidationLayerSupport();
};

#endif
