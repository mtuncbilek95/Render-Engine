cmake_minimum_required(VERSION 3.25)

if(CMAKE_BUILD_TYPE MATCHES Debug)
    add_compile_definitions(ENGINE_DEBUG)
endif()

add_executable(${PROJECT_NAME} "main.cpp" Application/Application.cpp Application/Application.hpp ValidationLayer/ValidationLayer.cpp ValidationLayer/ValidationLayer.hpp)

target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_SOURCE_DIR}/RenderEngine")
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_SOURCE_DIR}/External/GLFW/Include")
target_include_directories(${PROJECT_NAME} PUBLIC "C:/VulkanSDK/1.3.246.1/Include")

target_link_libraries(${PROJECT_NAME} "C:/VulkanSDK/1.3.246.1/Lib/vulkan-1.lib")
target_link_libraries(${PROJECT_NAME} "${CMAKE_SOURCE_DIR}/External/GLFW/Lib/glfw3.lib")