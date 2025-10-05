#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "file.hpp"

std::string readAsmFile(const std::string &filename)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "无法打开文件: " << filename << std::endl;
        return "";
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();

    return buffer.str();
}