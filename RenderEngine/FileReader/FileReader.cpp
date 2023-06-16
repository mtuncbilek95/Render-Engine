#include "FileReader.hpp"

#include <fstream>
#include <iostream>

#define STRING(x) #x"/"
#define XSTRING(x) STRING(x)


FileReader* FileReader::m_Instance = nullptr;

std::vector<char> FileReader::ReadShaderFile(std::string shaderName) {
    std::string shaderPath = XSTRING(RENDER_ROOT);
    shaderPath += "Shaders/" + shaderName + ".spv";

    std::ifstream file(shaderPath, std::ios::ate | std::ios::binary);

    if(!file.is_open())
        throw std::runtime_error("Failed to open Shader File!");

    size_t fileSize = (size_t)file.tellg();
    std::vector<char> buffer(fileSize);

    file.seekg(0);
    file.read(buffer.data(), fileSize);
    file.close();

    return buffer;
}

void FileReader::DeleteShaderFile(std::string shaderName) {
    std::string shaderPath = XSTRING(RENDER_ROOT);
    shaderPath += "Shaders/" + shaderName + ".spv";

    if (std::remove(shaderPath.c_str()) != 0) {
        // If the return value is non-zero, an error occurred
        perror("Error deleting file");
    }
}

void FileReader::CompileShader(std::string shaderName, std::string outName, ShaderType type) {

    std::string command = XSTRING(RENDER_ROOT);
    command += + "External/ShaderCompiler/glslc.exe ";
    command += XSTRING(RENDER_ROOT);
    command += "Shaders/" + shaderName;

    switch (type) {
        case ShaderType::Vertex:
            command += ".vert";
            break;
        case ShaderType::Geometry:
            command += ".geom";
            break;
        case ShaderType::Compute:
            command += ".comp";
            break;
        case ShaderType::Fragment:
            command += ".frag";
            break;
    }

    command += " -o ";
    command += XSTRING(RENDER_ROOT);
    command += "Shaders/" + outName + ".spv";

    std::system(command.c_str());
    std::system("exit");
}
