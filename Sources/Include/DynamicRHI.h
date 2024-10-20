#pragma once
#include<vector>
#include<functional>

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
		virtual void frameOnTick(std::function<void()> recreatefunc) = 0;

		//draw function
		virtual void drawIndexed(uint32_t index, uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, uint32_t vertexOffset, uint32_t firstInstance) = 0;

		//scissor
		virtual void setScissor(uint32_t index, uint32_t firstScissor, uint32_t scissorCount, DynamicRect2D rect) = 0;

		//clear functions
		virtual void clean() = 0;

		//command functions
		virtual void beginCommandBuffer(uint32_t index) = 0;
		virtual void endCommandBuffer(uint32_t index) = 0;
		virtual uint32_t getCommandBufferSize() = 0;

		//buffer functions
		virtual void bindVertexBuffers(DynamicBuffer* vertexBuffer, uint32_t index) = 0;
		virtual void bindIndexBuffer(DynamicBuffer* indexBuffer, uint32_t index) = 0;
		virtual uint32_t getCurrentBuffer() = 0;
		virtual void createDynamicBuffer(DynamicBuffer* buffer, DynamicDeviceMemory* deviceMemory, uint64_t bufferSize, void* bufferData, uint32_t usage) = 0;
		virtual void createUniformBuffer(std::vector<DynamicBuffer>* uniformBuffers, std::vector<DynamicDeviceMemory>* uniformBuffersMemory, std::vector<void*>* uniformBuffersMapped, uint32_t bufferSize) = 0;
		virtual void clearBuffer(DynamicBuffer* buffer, DynamicDeviceMemory* memory) = 0;
		virtual void flushBuffer(DynamicDeviceMemory* memory, uint32_t size, uint32_t offset) = 0;
		
		//memory functions
		virtual void mapMemory(DynamicDeviceMemory* memory, uint32_t offset, uint32_t size, void* data) = 0;
		virtual void unmapMemory(DynamicDeviceMemory* memory) = 0;

		//descriptor funcions
		virtual void createDescriptorPool(DynamicDescriptorPool* descriptorPool) = 0;
		virtual void createDescriptorSet(DynamicDescriptorSet* descriptorSet, DynamicDescriptorSetLayout* descriptorSetLayout, DynamicDescriptorPool* descriptorPool, std::vector<DynamicWriteDescriptorSet>* wds) = 0;
		virtual void bindDescriptorSets(DynamicDescriptorSet* descriptorSet, DynamicPipelineLayout pipelineLayout, uint32_t bindPoint, uint32_t index) = 0;
		virtual void createDescriptorSetLayout(DynamicDescriptorSetLayout* descriptorSetLayout, std::vector<DynamicDescriptorSetLayoutBinding>* dsbs) = 0;

		//texture funcitons
		virtual void createTextureImage(DynamicImage* textureImage, DynamicDeviceMemory* textureMemory, int texWidth, int texHeight, int texChannels, stbi_uc* pixels) = 0;
		virtual void createTextureSampler(DynamicSampler* textureSampler) = 0;
		
		//image functions
		virtual void createImageView(DynamicImageView* imageView, DynamicImage* image, uint32_t imageFormat) = 0;
		virtual void createImage(DynamicImage* image, uint32_t width, uint32_t height,
			uint32_t format, uint32_t imageTiling, uint32_t imageUsageFlagBits, uint32_t memoryPropertyFlags, DynamicDeviceMemory* imageMemory) = 0;
		virtual void copyBufferToImage(DynamicBuffer* buffer, DynamicImage* image, uint32_t width, uint32_t height) = 0;
		virtual void createSampler(DynamicSampler* sampler, DynamicSmplerCreateInfo createInfo) = 0;
		virtual void clearImage(DynamicSampler* sampler, DynamicImageView* imageView, DynamicImage* image, DynamicDeviceMemory* memory) = 0;

		//pipeline functions
		virtual void createPipelineLayout(DynamicPipelineLayout* pipelineLayout, DynamicPipelineLayoutCreateInfo* createInfo) = 0;
		virtual void createPipeline(DynamicPipeline* pipeline, DynamicPipelineLayout* pipelineLayout, PipelineCreateInfo info) = 0;
		virtual void bindPipeline(DynamicPipeline pipeline, uint32_t bindPoint, uint32_t index) = 0;   
	};
}