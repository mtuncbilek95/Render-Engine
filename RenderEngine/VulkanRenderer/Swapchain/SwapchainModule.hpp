#pragma once

#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"

#define GLFW_EXPOSE_NATIVE_WIN32
#include "GLFW/glfw3native.h"
#include <vector>

class SwapchainModule {
public:
    SwapchainModule();
    SwapchainModule(const SwapchainModule& other) = delete;
    SwapchainModule& operator=(const SwapchainModule& other) = delete;
    ~SwapchainModule();

    void CreateSwapchain(VkPhysicalDevice& physicalDevice);
    void CreateSurface(VkInstance& instance, GLFWwindow* window);

    VkSurfaceKHR GetSurface() const { return WindowSurface; }
    VkSwapchainKHR GetSwapchain() const { return Swapchain; }

private:
    VkSurfaceFormatKHR ChooseSwapSurfaceFormat(std::vector<VkSurfaceFormatKHR> formats);

private:
    VkSurfaceKHR WindowSurface;
    VkSwapchainKHR Swapchain;
};
