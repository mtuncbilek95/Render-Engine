#include "FileReader.hpp"

#include <fstream>
#include <iostream>

#define STRING(x) #x"/"
#define XSTRING(x) STRING(x)

FileReader* FileReader::m_Instance = nullptr;

std::vector<char> FileReader::ReadShaderFile(std::string path) {
    std::string shaderPath = XSTRING(RENDER_ROOT) + path;


    std::string command = XSTRING(RENDER_ROOT);
    command += "Shaders/Compile.bat";

    int result = std::system(command.c_str());

    if (result != 0) {
        throw std::runtime_error("Failed to open Shader File!");
    }

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

void FileReader::DeleteShaderFile(std::string path) {
    std::string shaderPath = XSTRING(RENDER_ROOT) + path;

    if (std::remove(shaderPath.c_str()) != 0) {
        // If the return value is non-zero, an error occurred
        perror("Error deleting file");
    }
}
