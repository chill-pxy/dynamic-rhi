#pragma once
#include<vector>

#include"InterfaceType.h"

namespace DRHI
{
	class DynamicRHI
	{
	public:
		//initialize vulkan rhi member
		virtual void initialize() = 0;
		//clean vulkan rhi member
		virtual void clean() = 0;
		//call vkCommandBegin function
		virtual void prepareCommandBuffer(DynamicBuffer* vertexBuffer, DynamicBuffer* indexBuffer, uint32_t indicesSize) = 0;
		//call within render loop
		virtual void frameOnTick() = 0;
		//Buffer class
		virtual void createDynamicBuffer(DynamicBuffer* vertexBuffer, DynamicDeviceMemory* deviceMemory, uint64_t bufferSize, void* bufferData) = 0;
		//unifrom buffer
		virtual void createUniformBuffer(std::vector<DynamicBuffer>* uniformBuffers, std::vector<DynamicDeviceMemory>* uniformBuffersMemory, std::vector<void*>* uniformBuffersMapped, uint32_t bufferSize) = 0;
	};
}