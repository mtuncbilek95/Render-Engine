#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#include <optional>
#include <vector>

// Using this to avoid having to pass the validation layers to every function that needs them.
struct QueueFamilyIndices {
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    bool IsComplete() {
        return graphicsFamily.has_value() && presentFamily.has_value();
    }
};

// Using this to avoid having to pass the swapchain support details to every function that needs them.
struct SwapchainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities{};
    std::vector<VkSurfaceFormatKHR> formats{};
    std::vector<VkPresentModeKHR> presentModes{};
};

class Application {
public:
    explicit Application(VkExtent2D windowSize);
    ~Application();

    void Run();

protected:
    void InitWindow();
    void MainLoop();
    void Cleanup();
    void DrawFrame();
protected:
    void InitVulkan();
    // Create an instance of the Vulkan API. This instance is used to interface with the Vulkan library.
    void CreateInstance();
    void CreateSurface();
    // Count the number of physical devices that support the Vulkan API. Store all the devices in a list. Select the best device that suits our needs.
    void PickPhysicalDevice();
    void CreateLogicalDevice();
    void CreateSwapchain();
    void CreateImageViews();
    void CreateRenderPass();
    void CreateGraphicsPipeline();
    void CreateFrameBuffers();
    void CreateCommandPool();
    void CreateCommandBuffers();
    void CreateSyncObjects();

protected:
    bool IsDeviceSuitable(VkPhysicalDevice device);
    QueueFamilyIndices  FindQueueFamilies(VkPhysicalDevice device);
    std::vector<const char*> GetRequiredExtensions();
private:
    GLFWwindow *m_Window;
    VkExtent2D m_WindowSize;

    VkSurfaceKHR m_Surface;
    VkSwapchainKHR m_Swapchain;

private:
    VkInstance m_Instance;
    VkPhysicalDevice m_PhysicalDevice;
    VkDevice m_Device;

    const std::vector<const char*> deviceExtensions = {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME,
    };
};
