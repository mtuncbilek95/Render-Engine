#pragma once

#include <string>
#include <vector>

class FileReader {
public:
    static FileReader& GetInstance(){
        if(!m_Instance)
            m_Instance = new FileReader();
        return *m_Instance;
    }

    std::vector<char> ReadShaderFile(std::string path);
    void DeleteShaderFile(std::string path);
private:
    static FileReader* m_Instance;
};
