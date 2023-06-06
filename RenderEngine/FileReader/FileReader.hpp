#pragma once

class FileReader {
public:
    static FileReader& GetInstance(){
        if(!m_Instance)
            m_Instance = new FileReader();
        return *m_Instance;
    }


private:
    static FileReader* m_Instance;
};
