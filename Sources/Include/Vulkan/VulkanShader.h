#pragma once

#include<vector>

#include"../InterfaceType.h"

namespace DRHI
{
    VkShaderModule createShaderModule(const std::vector<char>& code, VkDevice* device);
}