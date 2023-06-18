#include "EngineDevice.hpp"

#include "ValidationLayer/ValidationLayer.hpp"

void DeviceModule::CreateInstance() {
    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Vulkan Engine";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "Vulkan Engine";
    appInfo.engineVersion = VK_API_VERSION_1_3;
    appInfo.apiVersion = VK_API_VERSION_1_3;

    auto extensions = GetRequiredExtensions();

    VkInstanceCreateInfo instanceInfo{};
    instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instanceInfo.pApplicationInfo = &appInfo;
    instanceInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    instanceInfo.ppEnabledExtensionNames = extensions.data();

    if (ValidationLayer::GetInstance().IsValidationLayerEnabled()) {
        instanceInfo.enabledLayerCount = static_cast<uint32_t>(ValidationLayer::GetInstance().GetValidationLayers().size());
        instanceInfo.ppEnabledLayerNames = ValidationLayer::GetInstance().GetValidationLayers().data();
    } else {
        instanceInfo.enabledLayerCount = 0;
    }

    if (vkCreateInstance(&instanceInfo, nullptr, &VulkanInstance) != VK_SUCCESS)
        throw std::runtime_error("Failed to create Vulkan Instance!");
    else
        std::cout << "\033[0;36mSuccessfully created Vulkan Instance!\033[0;37m" << std::endl;


}

std::vector<const char*> DeviceModule::GetRequiredExtensions() {
    uint32_t glfwExtensionCount = 0;

    const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
    std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

    if (ValidationLayer::GetInstance().IsValidationLayerEnabled()) {
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

    return extensions;

}
