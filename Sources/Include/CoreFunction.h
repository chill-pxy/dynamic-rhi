#include<vector>
#include<string>
#include<fstream>
#include<stdexcept>
#include<array>

#include<glm/glm.hpp>
#include"InterfaceType.h"

namespace drhi
{
    //read file from address to generate std::vector<char> data
    static std::vector<char> readFile(const std::string& filename)
    {
        std::ifstream file(filename, std::ios::ate | std::ios::binary);

        if (!file.is_open()) {
            throw std::runtime_error("failed to open file!");
        }

        size_t fileSize = (size_t)file.tellg();
        std::vector<char> buffer(fileSize);

        file.seekg(0);
        file.read(buffer.data(), fileSize);

        file.close();

        return buffer;
    }
}