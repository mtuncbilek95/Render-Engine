#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>
#include <iostream>
#include <stdexcept>

class ValidationLayer {
public:
    static ValidationLayer &GetInstance() {
        if (instance == nullptr) {
            instance = new ValidationLayer();
        }
        return *instance;
    }

    std::vector<const char *> &GetValidationLayers() { return validationLayers; }
    bool IsValidationLayerEnabled();
    bool CheckValidationLayerSupport();
    void SetupDebugMessenger(VkInstance &vkInstance);
    VkResult CreateDebugUtilsMessengerEXT(
            VkInstance vkInstance,
            const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
            const VkAllocationCallbacks *pAllocator);
    void DestroyDebugUtilsMessengerEXT(
            VkInstance vkInstance,
            const VkAllocationCallbacks *pAllocator);
    void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &createInfo);
private:
    static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
            VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
            VkDebugUtilsMessageTypeFlagsEXT messageType,
            const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
            void *pUserData);

private:
    std::vector<const char *> validationLayers = {"VK_LAYER_KHRONOS_validation"};
    VkDebugUtilsMessengerEXT debugMessenger;
    bool enableValidationLayers;

private:
    ValidationLayer();
    static ValidationLayer *instance;
};