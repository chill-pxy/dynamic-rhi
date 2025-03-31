#pragma once

#include"../InterfaceType.h"

#include<stb_image.h>

namespace drhi
{
	namespace VulkanImage
	{
        void createTextureImage(VkImage* textureImage, VkDeviceMemory* textureMemory, int texWidth, int texHeight, int texChannels, stbi_uc* pixels, VkDevice* device, VkPhysicalDevice* physicalDevice, VkQueue* graphicsQueue, VkCommandPool* commandPool);

        void createTextureImage(VkImage* textureImage, DynamicImageCreateInfo info, VkDeviceMemory* textureMemory, stbi_uc* pixels, VkDevice* device, VkPhysicalDevice* physicalDevice, VkQueue* graphicsQueue, VkCommandPool* commandPool);

        void createCubeTextureImage(VkImage* image, VkDeviceMemory* deviceMemory, VkCommandPool cmdPool, unsigned char* textureData, uint64_t textureSize, uint32_t width, uint32_t height, uint32_t mipLevels, std::vector<size_t> offsets, std::vector<DynamicExtent2D> texSizes, VkDevice device, VkPhysicalDevice* physicalDevice, VkQueue copyQueue);

        void createImage(VkImage* image, DynamicImageCreateInfo info, VkDeviceMemory& imageMemory, VkMemoryPropertyFlags properties, VkDevice* device, VkPhysicalDevice* physicalDevice);

        void createImage(
            VkImage* image,
            uint32_t width, uint32_t height,
            VkFormat format, VkImageTiling tiling, VkSampleCountFlagBits samples,
            VkImageUsageFlags usage, VkMemoryPropertyFlags properties,
            VkDeviceMemory& imageMemory,
            VkDevice* device, VkPhysicalDevice* physicalDevice);

        void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, VkQueue* graphicsQueue, VkCommandPool* commandPool, VkDevice* device);
	
        void copyBufferToImage(VkBuffer* buffer, VkImage* image, uint32_t width, uint32_t height, VkQueue* graphicsQueue, VkCommandPool* commandPool, VkDevice* device);
    
        void copyBufferToImage(VkBuffer* buffer, VkImage* image, uint32_t width, uint32_t height, VkImageSubresourceRange range, VkQueue* graphicsQueue, VkCommandPool* commandPool, VkDevice* device);

        VkImageView createImageView(VkDevice* device, VkImage* image, VkFormat format, VkImageAspectFlags aspectFlags);
   
        VkImageView createImageView(VkDevice* device, VkImage* image, DynamicImageViewCreateInfo info);

        void createTextureSampler(VkSampler* textureSampler, VkPhysicalDevice* physicalDevice, VkDevice* device);
    
        void createSampler(VkSampler* sampler, DynamicSamplerCreateInfo createInfo, VkPhysicalDevice* physicalDevice, VkDevice* device);
    
        void setImageLayout(
            VkCommandBuffer cmdbuffer,
            VkImage& image,
            VkImageLayout oldImageLayout,
            VkImageLayout newImageLayout,
            VkImageSubresourceRange subresourceRange,
            VkPipelineStageFlags srcStageMask,
            VkPipelineStageFlags dstStageMask);
    }
}
