#pragma once
#include<vector>

#include<stb_image.h>

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
		//create discriptor set
		virtual void createDescriptorSets(std::vector<DynamicBuffer>* uniformBuffers, uint32_t uniformBufferSize, DynamicImageView textureImageView, DynamicSampler textureSampler) = 0;
		//texture
		virtual void createTextureImage(DynamicImage* textureImage, DynamicDeviceMemory* textureMemory, int texWidth, int texHeight, int texChannels, stbi_uc* pixels) = 0;
		virtual void createImageView(DynamicImageView* imageView, DynamicImage* image) = 0;
		virtual void createTextureSampler(DynamicSampler* textureSampler) = 0;
	};
}