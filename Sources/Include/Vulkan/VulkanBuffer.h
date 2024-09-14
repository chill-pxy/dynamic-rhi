#pragma once

#include<stdexcept>

#include"../InterfaceType.h"

#include"VulkanCommand.h"

namespace DRHI
{
    namespace VulkanBuffer
    {
        uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties, VkPhysicalDevice* physicalDevice);

        void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size, VkCommandPool* commandPool, VkDevice* device, VkQueue* graphicsQueue);

        void createBuffer(VkDevice* device, VkPhysicalDevice* physicalDevice, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer* buffer, VkDeviceMemory* bufferMemory);

        void createDynamicBuffer(DynamicBuffer* buffer, DynamicDeviceMemory* deviceMemory, uint64_t bufferSize, void* bufferData, VkDevice* device, VkPhysicalDevice* physicalDevice, VkCommandPool* commandPool, VkQueue* graphicsQueue, const char* type);
    }
}