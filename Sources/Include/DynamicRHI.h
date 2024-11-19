#pragma once
#include<vector>
#include<functional>

#include<stb_image.h>

#include"InterfaceType.h"

namespace DRHI
{
	class DynamicRHI
	{
	public:
		//get current api type
		virtual API getCurrentAPI() { return API::VULKAN; };

		//initialize function
		virtual void initialize() = 0;

		//tick function
		virtual void frameOnTick(std::vector<std::function<void()>> recreatefuncs, std::vector<DynamicCommandBuffer>* commandBuffers) = 0;

		//draw function
		virtual void drawIndexed(DynamicCommandBuffer* commandBuffer, uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, uint32_t vertexOffset, uint32_t firstInstance) = 0;

		//scissor
		virtual void setScissor(DynamicCommandBuffer* commandBuffer, uint32_t firstScissor, uint32_t scissorCount, DynamicRect2D rect) = 0;

		//clear functions
		virtual void clean() = 0;

		//get frame
		virtual uint32_t getCurrentFrame() = 0;

		//command functions
		virtual void createCommandPool(DynamicCommandPool* commandPool) = 0;
		virtual void createCommandBuffers(std::vector<DynamicCommandBuffer>* commandBuffers, DynamicCommandPool* commandPool) = 0;
		virtual void beginCommandBuffer(DynamicCommandBuffer commandBuffer) = 0;
		virtual void beginRendering(DynamicCommandBuffer commandBuffer,  bool isClear, uint32_t index) = 0;
		virtual void beginRendering(DynamicCommandBuffer commandBuffer, bool isClear) = 0;
		virtual void endCommandBuffer(DynamicCommandBuffer commandBuffer) = 0;
		virtual void endRendering(DynamicCommandBuffer commandBuffer, uint32_t index) = 0;
		virtual void endRendering(DynamicCommandBuffer commandBuffer) = 0;
		virtual void freeCommandBuffers(std::vector<DynamicCommandBuffer>* commandBuffers, DynamicCommandPool* commandPool) = 0;

		//buffer functions
		virtual void bindVertexBuffers(DynamicBuffer* vertexBuffer, DynamicCommandBuffer* commandBuffer) = 0;
		virtual void bindIndexBuffer(DynamicBuffer* indexBuffer, DynamicCommandBuffer* commandBuffer, uint32_t indexType) = 0;
		virtual void createDynamicBuffer(DynamicBuffer* buffer, DynamicDeviceMemory* deviceMemory, DynamicCommandPool* commandPool, uint64_t bufferSize, void* bufferData, uint32_t usage, uint32_t memoryProperty) = 0;
		virtual void createUniformBuffer(std::vector<DynamicBuffer>* uniformBuffers, std::vector<DynamicDeviceMemory>* uniformBuffersMemory, std::vector<void*>* uniformBuffersMapped, uint32_t bufferSize) = 0;
		virtual void createUniformBuffer(DynamicBuffer* uniformBuffer, DynamicDeviceMemory* uniformBufferMemory, void** uniformBufferMapped, uint32_t bufferSize) = 0;
		//virtual void createDynamicBuffer(DynamicBuffer* buffer, DynamicDeviceMemory* memory, uint64_t bufferSize, uint32_t usage);
		virtual void clearBuffer(DynamicBuffer* buffer, DynamicDeviceMemory* memory) = 0;
		virtual void flushBuffer(DynamicDeviceMemory* memory, uint32_t size, uint32_t offset) = 0;
		virtual void flushBuffer(DynamicDeviceMemory* memory, uint32_t offset) = 0;

		//memory functions
		virtual void mapMemory(DynamicDeviceMemory* memory, uint32_t offset, uint32_t size, void* data) = 0;
		virtual void unmapMemory(DynamicDeviceMemory* memory) = 0;

		//descriptor funcions
		virtual void createDescriptorPool(DynamicDescriptorPool* descriptorPool, std::vector<DynamicDescriptorPoolSize>* poolsizes) = 0;
		virtual void createDescriptorPool(DynamicDescriptorPool* descriptorPool) = 0;
		virtual void createDescriptorPool(DynamicDescriptorPool* descriptorPool, DynamicDescriptorPoolCreateInfo* ci) = 0;
		virtual void createDescriptorSet(DynamicDescriptorSet* descriptorSet, DynamicDescriptorSetLayout* descriptorSetLayout, DynamicDescriptorPool* descriptorPool, std::vector<DynamicWriteDescriptorSet>* wds) = 0;
		virtual void bindDescriptorSets(DynamicDescriptorSet* descriptorSet, DynamicPipelineLayout pipelineLayout, DynamicCommandBuffer* commandBuffer, uint32_t bindPoint) = 0;
		virtual void createDescriptorSetLayout(DynamicDescriptorSetLayout* descriptorSetLayout, std::vector<DynamicDescriptorSetLayoutBinding>* dsbs) = 0;

		//texture funcitons
		virtual void createTextureImage(DynamicImage* textureImage, DynamicDeviceMemory* textureMemory, DynamicCommandPool* commandPool, int texWidth, int texHeight, int texChannels, stbi_uc* pixels) = 0;
		virtual void createTextureSampler(DynamicSampler* textureSampler) = 0;

		//image functions
		virtual void createImageView(DynamicImageView* imageView, DynamicImage* image, uint32_t imageFormat, uint32_t imageAspect) = 0;
		virtual void createImage(DynamicImage* image, uint32_t width, uint32_t height,
			uint32_t format, uint32_t imageTiling, uint32_t imageUsageFlagBits, uint32_t memoryPropertyFlags, DynamicDeviceMemory* imageMemory) = 0;
		virtual void copyBufferToImage(DynamicBuffer* buffer, DynamicImage* image, DynamicCommandPool* commandPool, uint32_t width, uint32_t height) = 0;
		virtual void createSampler(DynamicSampler* sampler, DynamicSmplerCreateInfo createInfo) = 0;
		virtual void clearImage(DynamicSampler* sampler, DynamicImageView* imageView, DynamicImage* image, DynamicDeviceMemory* memory) = 0;
		virtual void createViewportImage(std::vector<DynamicImage>* viewportImages, std::vector<DynamicDeviceMemory>* viewportImageMemorys, DynamicCommandPool* commandPool) = 0;
		virtual void createViewportImageViews(std::vector<DynamicImageView>* viewportImageViews, std::vector<DynamicImage>* viewportImages) = 0;

		//pipeline functions
		virtual void createPipelineLayout(DynamicPipelineLayout* pipelineLayout, DynamicPipelineLayoutCreateInfo* createInfo) = 0;
		virtual void createPipeline(DynamicPipeline* pipeline, DynamicPipelineLayout* pipelineLayout, DynamicPipelineCreateInfo info) = 0;
		virtual void bindPipeline(DynamicPipeline pipeline, DynamicCommandBuffer* commandBuffer, uint32_t bindPoint) = 0;

		//cmd functions
		virtual void cmdPushConstants(DynamicPipelineLayout* layout, DynamicCommandBuffer* commandBuffer, uint32_t stage, uint32_t offset, uint32_t size, void* value) = 0;
	};
}