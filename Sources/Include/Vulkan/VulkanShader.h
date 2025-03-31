#pragma once

#include<vector>

#include"../InterfaceType.h"

namespace drhi
{
    VkShaderModule createShaderModule(const std::vector<char>& code, VkDevice* device);
}