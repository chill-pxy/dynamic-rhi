#pragma once
#include<vector>

#include<stb_image.h>

#include"InterfaceType.h"

namespace DRHI
{
	struct PipelineCreateInfo
	{
		const char* vertexShader;
		const char* fragmentShader;
		DynamicVertexInputBindingDescription vertexInputBinding;
		std::vector<DynamicVertexInputAttributeDescription> vertexInputAttributes;
	};

	class DynamicRHI
	{
	public:
		//initialize vulkan rhi member
		virtual void initialize() = 0;
		//clean vulkan rhi member
		virtual void clean(std::vector<DynamicBuffer>* uniformBuffers, std::vector <DynamicDeviceMemory>* uniformBuffersMemory, DynamicImageView* textureImageView, DynamicSampler* textureSampler,
			DynamicImage* textureImage, DynamicDeviceMemory* textureImageMemory, DynamicBuffer* indexBuffer, DynamicDeviceMemory* indexBufferMemory, DynamicBuffer* vertexBuffer, DynamicDeviceMemory* vertexBufferMemory) = 0;
		//call vkCommandBegin function
		virtual void prepareCommandBuffer(DynamicBuffer* vertexBuffer, DynamicBuffer* indexBuffer, uint32_t indicesSize) = 0;
		//call within render loop
		virtual void frameOnTick() = 0;
		//get commandBuffersize
		virtual uint32_t getCommandBufferSize() = 0;
		//get current buffer
		virtual uint32_t getCurrentBuffer() = 0;
		//Buffer class
		virtual void createDynamicBuffer(DynamicBuffer* vertexBuffer, DynamicDeviceMemory* deviceMemory, uint64_t bufferSize, void* bufferData, const char* type) = 0;
		//unifrom buffer
		virtual void createUniformBuffer(std::vector<DynamicBuffer>* uniformBuffers, std::vector<DynamicDeviceMemory>* uniformBuffersMemory, std::vector<void*>* uniformBuffersMapped, uint32_t bufferSize) = 0;
		//create discriptor set
		virtual void createDescriptorSet(std::vector<DynamicDescriptorBufferInfo>* descriptor, DynamicImageView textureImageView, DynamicSampler textureSampler) = 0;
		//texture
		virtual void createTextureImage(DynamicImage* textureImage, DynamicDeviceMemory* textureMemory, int texWidth, int texHeight, int texChannels, stbi_uc* pixels) = 0;
		virtual void createImageView(DynamicImageView* imageView, DynamicImage* image) = 0;
		virtual void createTextureSampler(DynamicSampler* textureSampler) = 0;
		//create the particular pipeline
		virtual void createPipeline(PipelineCreateInfo info) = 0;
	};
}