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
		//get current api type
		virtual API getCurrentAPI() = 0;

		//initialize function
		virtual void initialize() = 0;

		//tick function
		virtual void frameOnTick() = 0;

		//draw function
		virtual void drawIndexed(uint32_t index, uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, uint32_t vertexOffset, uint32_t firstInstance) = 0;

		//clear functions
		virtual void clean() = 0;
		virtual void clearImage(DynamicSampler* sampler, DynamicImageView* imageView, DynamicImage* image, DynamicDeviceMemory* memory) = 0;
		virtual void clearBuffer(DynamicBuffer* buffer, DynamicDeviceMemory* memory) = 0;

		//command functions
		virtual void beginCommandBuffer(uint32_t index) = 0;
		virtual void endCommandBuffer(uint32_t index) = 0;
		virtual uint32_t getCommandBufferSize() = 0;

		//buffer functions
		virtual void bindVertexBuffers(DynamicBuffer* vertexBuffer, uint32_t index) = 0;
		virtual void bindIndexBuffer(DynamicBuffer* indexBuffer, uint32_t index) = 0;
		virtual uint32_t getCurrentBuffer() = 0;
		virtual void createDynamicBuffer(DynamicBuffer* vertexBuffer, DynamicDeviceMemory* deviceMemory, uint64_t bufferSize, void* bufferData, const char* type) = 0;
		virtual void createUniformBuffer(std::vector<DynamicBuffer>* uniformBuffers, std::vector<DynamicDeviceMemory>* uniformBuffersMemory, std::vector<void*>* uniformBuffersMapped, uint32_t bufferSize) = 0;

		//descriptor funcions
		virtual void createDescriptorSet(DynamicDescriptorSet* descriptorSet, DynamicDescriptorSetLayout* descriptorSetLayout, std::vector<DynamicDescriptorBufferInfo>* descriptor, DynamicImageView textureImageView, DynamicSampler textureSampler) = 0;
		virtual void bindDescriptorSets(DynamicDescriptorSet* descriptorSet, DynamicPipelineLayout pipelineLayout, uint32_t bindPoint, uint32_t index) = 0;
		virtual void createDescriptorSetLayout(DynamicDescriptorSetLayout* descriptorSetLayout) = 0;

		//texture funcitons
		virtual void createTextureImage(DynamicImage* textureImage, DynamicDeviceMemory* textureMemory, int texWidth, int texHeight, int texChannels, stbi_uc* pixels) = 0;
		virtual void createImageView(DynamicImageView* imageView, DynamicImage* image) = 0;
		virtual void createTextureSampler(DynamicSampler* textureSampler) = 0;

		//pipeline functions
		virtual void createPipeline(DynamicPipeline* pipeline, DynamicPipelineLayout* pipelineLayout, DynamicDescriptorSetLayout* descriptorSetLayout, PipelineCreateInfo info) = 0;
		virtual void bindPipeline(DynamicPipeline pipeline, uint32_t bindPoint, uint32_t index) = 0;   
	};
}