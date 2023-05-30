#include "Application.hpp"

#include <ValidationLayer/ValidationLayer.hpp>

#include <iostream>
#include <set>

Application::Application(VkExtent2D windowSize) : m_WindowSize(windowSize), m_Window(nullptr), m_Instance(nullptr), m_PhysicalDevice(VK_NULL_HANDLE) {

}

Application::~Application() {
    Cleanup();
}

void Application::Run() {
    InitWindow();
    InitVulkan();
    MainLoop();
}

void Application::InitWindow() {
    if (!glfwInit()) {
        throw std::runtime_error("Failed to initialize GLFW!");
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    m_Window = glfwCreateWindow(m_WindowSize.width, m_WindowSize.height, "Render Engine", nullptr, nullptr);
}

void Application::MainLoop() {
    while (!glfwWindowShouldClose(m_Window)) {
        glfwPollEvents();
        DrawFrame();
    }
}

void Application::Cleanup() {
    vkDestroyDevice(m_Device, nullptr);

    if (ValidationLayer::GetInstance().IsValidationLayerEnabled()) {
        ValidationLayer::GetInstance().DestroyDebugUtilsMessengerEXT(m_Instance, nullptr);
    }
    vkDestroySurfaceKHR(m_Instance, m_Surface, nullptr);
    vkDestroyInstance(m_Instance, nullptr);
    glfwDestroyWindow(m_Window);
    glfwTerminate();
}

void Application::InitVulkan() {
    CreateInstance();
    ValidationLayer::GetInstance().SetupDebugMessenger(m_Instance);
    CreateSurface();
    PickPhysicalDevice();
    CreateLogicalDevice();
    CreateSwapchain();
    CreateImageViews();
    CreateRenderPass();
    CreateGraphicsPipeline();
    CreateFrameBuffers();
    CreateCommandPool();
    CreateCommandBuffers();
    CreateSyncObjects();
}

void Application::CreateInstance() {
    if (ValidationLayer::GetInstance().IsValidationLayerEnabled() && !ValidationLayer::GetInstance().CheckValidationLayerSupport()) {
        throw std::runtime_error("Validation layers requested, but not available!");
    }
    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Render Engine";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "Render Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_3;

    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    auto extensions = GetRequiredExtensions();

    createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    createInfo.ppEnabledExtensionNames = extensions.data();

    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
    if (ValidationLayer::GetInstance().IsValidationLayerEnabled()) {
        createInfo.enabledLayerCount = static_cast<uint32_t>(ValidationLayer::GetInstance().GetValidationLayers().size());
        createInfo.ppEnabledLayerNames = ValidationLayer::GetInstance().GetValidationLayers().data();

        ValidationLayer::GetInstance().PopulateDebugMessengerCreateInfo(debugCreateInfo);
        createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT *) &debugCreateInfo;
    } else {
        createInfo.enabledLayerCount = 0;
        createInfo.pNext = nullptr;
    }

    if (vkCreateInstance(&createInfo, nullptr, &m_Instance) != VK_SUCCESS)
        throw std::runtime_error("Failed to create Vulkan Instance!");
    else
        std::cout << "\033[0;36mSuccessfully created Vulkan Instance!\033[0;37m" << std::endl;

}

void Application::CreateSurface() {
    VkWin32SurfaceCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
    createInfo.hwnd = glfwGetWin32Window(m_Window);
    createInfo.hinstance = GetModuleHandle(nullptr);


    if (vkCreateWin32SurfaceKHR(m_Instance, &createInfo, nullptr, &m_Surface) != VK_SUCCESS)
        throw std::runtime_error("Failed to create Window Surface!");
    else
        std::cout << "\033[0;36mSuccessfully created Window Surface!\033[0;37m" << std::endl;

}

void Application::PickPhysicalDevice() {
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(m_Instance, &deviceCount, nullptr);

    if (deviceCount == 0) {
        throw std::runtime_error("Failed to find GPUs with Vulkan support!");
    }

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(m_Instance, &deviceCount, devices.data());

    for (const auto &device: devices) {
        if (IsDeviceSuitable(device)) {
            m_PhysicalDevice = device;
            break;
        }
    }

    if(m_PhysicalDevice == VK_NULL_HANDLE)
        throw std::runtime_error("Failed to find a suitable GPU!");
}

void Application::CreateLogicalDevice() {
    QueueFamilyIndices indices = FindQueueFamilies(m_PhysicalDevice);

    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    std::set<uint32_t> uniqueQueueFamilies = {indices.graphicsFamily.value(), indices.presentFamily.value()};

    float queuePriority = 1.0f;
    for (uint32_t queueFamily : uniqueQueueFamilies) {
        VkDeviceQueueCreateInfo queueCreateInfo{};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = queueFamily;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;
        queueCreateInfos.push_back(queueCreateInfo);
    }

    VkPhysicalDeviceFeatures deviceFeatures{};

    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

    createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
    createInfo.pQueueCreateInfos = queueCreateInfos.data();

    createInfo.pEnabledFeatures = &deviceFeatures;
    createInfo.enabledExtensionCount = 0;

    if(ValidationLayer::GetInstance().IsValidationLayerEnabled()) {
        createInfo.enabledLayerCount = static_cast<uint32_t>(ValidationLayer::GetInstance().GetValidationLayers().size());
        createInfo.ppEnabledLayerNames = ValidationLayer::GetInstance().GetValidationLayers().data();
    } else {
        createInfo.enabledLayerCount = 0;
    }

    if(vkCreateDevice(m_PhysicalDevice, &createInfo, nullptr, &m_Device) != VK_SUCCESS)
        throw std::runtime_error("Failed to create Logical Device!");
    else
        std::cout << "\033[0;36mSuccessfully created Logical Device!\033[0;37m" << std::endl;

    vkGetDeviceQueue(m_Device, indices.graphicsFamily.value(), 0, &m_GraphicsQueue);
    vkGetDeviceQueue(m_Device, indices.presentFamily.value(), 0, &m_PresentQueue);
}

void Application::CreateSwapchain() {

}

void Application::CreateImageViews() {

}

void Application::CreateRenderPass() {

}

void Application::CreateGraphicsPipeline() {

}

void Application::CreateFrameBuffers() {

}

void Application::CreateCommandPool() {

}

void Application::CreateCommandBuffers() {

}

void Application::CreateSyncObjects() {

}

void Application::DrawFrame() {

}

std::vector<const char *> Application::GetRequiredExtensions() {
    uint32_t glfwExtensionCount{};
    const char **glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    std::vector<const char *> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

    if (ValidationLayer::GetInstance().IsValidationLayerEnabled()) {
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

    return extensions;
}

bool Application::IsDeviceSuitable(VkPhysicalDevice device) {

    QueueFamilyIndices indices = FindQueueFamilies(device);

    return indices.IsComplete();
}

QueueFamilyIndices Application::FindQueueFamilies(VkPhysicalDevice device) {
    uint32_t queueFamilyCount{};
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

    QueueFamilyIndices indices;

    for (size_t i{}; i < queueFamilies.size(); ++i) {
        if(queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
            indices.graphicsFamily = static_cast<uint32_t>(i);

        VkBool32 presentSupport{};
        vkGetPhysicalDeviceSurfaceSupportKHR(device, static_cast<uint32_t>(i), m_Surface, &presentSupport);

        if (presentSupport)
            indices.presentFamily = static_cast<uint32_t>(i);
        if(presentSupport)
            indices.presentFamily = static_cast<uint32_t>(i);
        if (indices.IsComplete())
            break;
    }

    return indices;
}
