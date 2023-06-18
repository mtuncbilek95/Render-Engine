#pragma

#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"

#include <vector>

class DeviceModule {
public:
    DeviceModule();
    DeviceModule(const DeviceModule& other) = delete;
    DeviceModule& operator=(const DeviceModule& other) = delete;
    ~DeviceModule();

    void CreateInstance();

private:
    std::vector<const char*> GetRequiredExtensions();
private:
    VkInstance VulkanInstance;
    VkPhysicalDevice PhysicalDevice;
    VkDevice VulkanDevice;
};

