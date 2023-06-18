#include "SwapchainModule.hpp"

#include <stdexcept>
#include <iostream>

#include "VulkanRenderer/Common/VulkanCommon.hpp"

void SwapchainModule::CreateSwapchain(VkPhysicalDevice& physicalDevice) {
    SwapchainSupportDetails swapchainSupport = VulkanBase::GetInstance().QuerySwapchainSupport(physicalDevice, WindowSurface);
    VkSurfaceFormatKHR surfaceFormat = ChooseSwapSurfaceFormat(swapchainSupport.Formats);

}

void SwapchainModule::CreateSurface(VkInstance& instance, GLFWwindow* window) {
    VkWin32SurfaceCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
    createInfo.hwnd = glfwGetWin32Window(window);
    createInfo.hinstance = GetModuleHandle(nullptr);

    if (vkCreateWin32SurfaceKHR(instance, &createInfo, nullptr, &WindowSurface) != VK_SUCCESS)
        throw std::runtime_error("Failed to create Window Surface!");
    else
        std::cout << "\033[0;36mSuccessfully created Window Surface!\033[0;37m" << std::endl;
}

VkSurfaceFormatKHR SwapchainModule::ChooseSwapSurfaceFormat(std::vector<VkSurfaceFormatKHR> formats) {
    for (const auto& format : formats) {
        if (format.format == VK_FORMAT_B8G8R8A8_SRGB &&
            format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
            return format;
    }

    return formats[0];
}
