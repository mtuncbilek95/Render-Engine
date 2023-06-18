#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>

class GraphicsPipeline {
public:
    GraphicsPipeline(VkDevice device);
    GraphicsPipeline(const GraphicsPipeline& other) = delete;
    GraphicsPipeline& operator=(const GraphicsPipeline& other) = delete;
    ~GraphicsPipeline();

    void CreateGraphicsPipeline();
    void DestroyGraphicsPipeline();

    VkPipelineShaderStageCreateInfo CreateShaderStage(VkShaderStageFlagBits stage, VkShaderModule shaderModule);

private:
    VkShaderModule CreateShaderModule(std::vector<char>& code);
    void CreatePipelineLayout();
    void CreatePipeline();

private:
    VkDevice* m_Device;

    VkPipelineLayout m_PipelineLayout;
    VkPipeline m_GraphicsPipeline;
};

