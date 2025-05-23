#include"../../Include/Vulkan/VulkanBuffer.h"

namespace drhi
{
    namespace VulkanBuffer
    {
        uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties, VkPhysicalDevice* physicalDevice)
        {
            VkPhysicalDeviceMemoryProperties memProperties;
            vkGetPhysicalDeviceMemoryProperties(*physicalDevice, &memProperties);

            for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
            {
                if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
                {
                    return i;
                }
            }

            throw std::runtime_error("failed to find suitable memory type!");
        }

        void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size, VkCommandPool* commandPool, VkDevice* device, VkQueue* graphicsQueue)
        {
            VkCommandBuffer commandBuffer = VulkanCommand::beginSingleTimeCommands(commandPool, device);

            VkBufferCopy copyRegion{};
            copyRegion.size = size;
            vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

            VulkanCommand::endSingleTimeCommands(commandBuffer, graphicsQueue, commandPool, device);
        }

        void createBuffer(VkDevice* device, VkPhysicalDevice* physicalDevice, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer* buffer, VkDeviceMemory* bufferMemory)
        {
            VkBufferCreateInfo bufferInfo{};
            bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
            bufferInfo.size = size;
            bufferInfo.usage = usage;
            bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

            if (vkCreateBuffer(*device, &bufferInfo, nullptr, buffer) != VK_SUCCESS) {
                throw std::runtime_error("failed to create buffer!");
            }

            VkMemoryRequirements memRequirements;
            vkGetBufferMemoryRequirements(*device, *buffer, &memRequirements);

            VkMemoryAllocateInfo allocInfo{};
            allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
            allocInfo.allocationSize = memRequirements.size;
            allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties, physicalDevice);

            if (vkAllocateMemory(*device, &allocInfo, nullptr, bufferMemory) != VK_SUCCESS) {
                throw std::runtime_error("failed to allocate buffer memory!");
            }

            vkBindBufferMemory(*device, *buffer, *bufferMemory, 0);
        }

        void createDynamicBuffer(DynamicBuffer* buffer, DynamicDeviceMemory* deviceMemory, uint64_t bufferSize, void* bufferData, VkDevice* device, VkPhysicalDevice* physicalDevice, VkCommandPool* commandPool, VkQueue* graphicsQueue, uint32_t usage, uint32_t memoryProperty)
        {
            VkBuffer stagingBuffer;
            VkDeviceMemory stagingBufferMemory;
            createBuffer(device, physicalDevice, bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, &stagingBuffer, &stagingBufferMemory);

            void* data;
            vkMapMemory(*device, stagingBufferMemory, 0, bufferSize, 0, &data);
            memcpy(data, bufferData, (size_t)bufferSize);
            vkUnmapMemory(*device, stagingBufferMemory);

            VkBuffer inputBuffer;
            VkDeviceMemory inputMemory;

            createBuffer(device, physicalDevice, bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | (VkBufferUsageFlagBits)usage, (VkMemoryPropertyFlags)memoryProperty, &inputBuffer, &inputMemory);

            copyBuffer(stagingBuffer, inputBuffer, bufferSize, commandPool, device, graphicsQueue);

            buffer->internalID = inputBuffer;
            deviceMemory->internalID = inputMemory;

            vkDestroyBuffer(*device, stagingBuffer, nullptr);
            vkFreeMemory(*device, stagingBufferMemory, nullptr);
        }

        //void createDynamicBuffer(DynamicBuffer* buffer, DynamicDeviceMemory* memory, uint64_t bufferSize, uint32_t usage, VkPhysicalDevice* physicalDevice, VkDevice* device)
        //{
        //    VkBuffer vkbuffer{};
        //    VkDeviceMemory vkbufferMemory{};
        //    createBuffer(device, physicalDevice, bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | (VkBufferUsageFlagBits)usage, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, &vkbuffer, &vkbufferMemory);

        //    buffer->internalID = vkbuffer;
        //    memory->internalID = vkbufferMemory;
        //}
    }
}