#pragma once

#include<stb_image.h>

#include"../InterfaceType.h"

namespace DRHI
{
	namespace VulkanImage
	{
        void createTextureImage(VkImage* textureImage, int texWidth, int texHeight, stbi_uc* pixels, VkDevice* device, VkPhysicalDevice* physicalDevice, VkQueue* graphicsQueue, VkCommandPool* commandPool);

        void createImage(
            VkImage* image,
            uint32_t width, uint32_t height,
            VkFormat format, VkImageTiling tiling,
            VkImageUsageFlags usage, VkMemoryPropertyFlags properties,
            VkDeviceMemory& imageMemory,
            VkDevice* device, VkPhysicalDevice* physicalDevice);

        void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, VkQueue* graphicsQueue, VkCommandPool* commandPool, VkDevice* device);
	
        void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height, VkQueue* graphicsQueue, VkCommandPool* commandPool, VkDevice* device);
    }
}
