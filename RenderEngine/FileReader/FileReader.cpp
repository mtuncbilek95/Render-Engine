#include "FileReader.hpp"

#include <fstream>

#define STRING(x) #x"/"
#define XSTRING(x) STRING(x)

FileReader* FileReader::m_Instance = nullptr;

std::vector<char> FileReader::ReadShaderFile(std::string path) {
    std::string shaderPath = XSTRING(RENDER_ROOT) + path;

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
