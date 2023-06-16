#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

class GraphicsPipeline {
public:
    GraphicsPipeline();
    ~GraphicsPipeline();

    void CreateGraphicsPipeline();

private:
    void CreateShaderModules();
    void CreatePipelineLayout();
    void CreatePipeline();
};

