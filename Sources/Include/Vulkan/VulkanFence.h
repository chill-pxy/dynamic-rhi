#pragma once

#include<vector>
#include<stdexcept>

#include"../InterfaceType.h"

namespace drhi
{
	void createSynchronizationPrimitives(std::vector<VkFence>* waitFences, uint32_t commandBufferSize, VkDevice* device);
}