#pragma once

#include <string>
#include <vector>

enum class ShaderType {
    Vertex,
    Geometry,
    Compute,
    Fragment
};

class FileReader {
public:
    static FileReader& GetInstance(){
        if(!m_Instance)
            m_Instance = new FileReader();
        return *m_Instance;
    }
    void CompileShader(std::string shaderName, std::string outName, ShaderType type);
    std::vector<char> ReadShaderFile(std::string shaderName);
    void DeleteShaderFile(std::string shaderName);

private:
    static FileReader* m_Instance;
};
