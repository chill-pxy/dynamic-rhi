#include<array>
#include<stdexcept>
#include<vector>

#include"../../Include/Vulkan/VulkanDescriptor.h"

namespace DRHI
{
    namespace VulkanDescriptor
    {
        void createDescriptorSetLayout(VkDescriptorSetLayout* descriptorSetLayout, VkDevice* device)
        {
            VkDescriptorSetLayoutBinding uboLayoutBinding{};
            uboLayoutBinding.binding = 0;
            uboLayoutBinding.descriptorCount = 1;
            uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            uboLayoutBinding.pImmutableSamplers = nullptr;
            uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

            VkDescriptorSetLayoutBinding samplerLayoutBinding{};
            samplerLayoutBinding.binding = 1;
            samplerLayoutBinding.descriptorCount = 1;
            samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
            samplerLayoutBinding.pImmutableSamplers = nullptr;
            samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

            std::array<VkDescriptorSetLayoutBinding, 2> bindings = { uboLayoutBinding, samplerLayoutBinding };
            VkDescriptorSetLayoutCreateInfo layoutInfo{};
            layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
            layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
            layoutInfo.pBindings = bindings.data();

            if (vkCreateDescriptorSetLayout(*device, &layoutInfo, nullptr, descriptorSetLayout) != VK_SUCCESS) {
                throw std::runtime_error("failed to create descriptor set layout!");
            }
        }

        void createDescriptorPool(VkDescriptorPool* descriptorPool, VkDevice* device)
        {
            std::array<VkDescriptorPoolSize, 2> poolSizes{};
            poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            poolSizes[0].descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
            poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
            poolSizes[1].descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);

            VkDescriptorPoolCreateInfo poolInfo{};
            poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
            poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
            poolInfo.pPoolSizes = poolSizes.data();
            poolInfo.maxSets = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);

            if (vkCreateDescriptorPool(*device, &poolInfo, nullptr, descriptorPool) != VK_SUCCESS) {
                throw std::runtime_error("failed to create descriptor pool!");
            }
        }

        void createDescriptorSet(VkDescriptorSet* descriptorSet, VkDescriptorPool* descriptorPool, VkDescriptorSetLayout* descriptorSetLayout, uint32_t descriptorSetCount, VkDevice* device, VkDescriptorBufferInfo* uniformBufferInfo)
        {
            VkDescriptorSetAllocateInfo descriptorSetAllocateInfo{};
            descriptorSetAllocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
            descriptorSetAllocateInfo.descriptorPool = *descriptorPool;
            descriptorSetAllocateInfo.pSetLayouts = descriptorSetLayout;
            descriptorSetAllocateInfo.descriptorSetCount = descriptorSetCount;
            if (vkAllocateDescriptorSets(*device, &descriptorSetAllocateInfo, descriptorSet))
            {
                throw std::runtime_error("failed to allocate descriptorsets");
            }

            VkWriteDescriptorSet writeDescriptorSet{};
            writeDescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            writeDescriptorSet.dstSet = *descriptorSet;
            writeDescriptorSet.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            writeDescriptorSet.dstBinding = 0;
            writeDescriptorSet.pBufferInfo = uniformBufferInfo;
            writeDescriptorSet.descriptorCount = 1;

            vkUpdateDescriptorSets(*device, 1, &writeDescriptorSet, 0, nullptr);
        }


        void createDescriptorSets(std::vector<VkDescriptorSet>* descriptorSets, VkDescriptorSetLayout* descriptorSetLayout, 
            VkDescriptorPool* descriptorPool, VkDevice* device, 
            std::vector<DynamicBuffer>* uniformBuffers, uint32_t uniformBufferSize,
            VkImageView* textureImageView, VkSampler* textureSampler)
        {
            std::vector<VkDescriptorSetLayout> layouts(MAX_FRAMES_IN_FLIGHT, *descriptorSetLayout);
            VkDescriptorSetAllocateInfo allocInfo{};
            allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
            allocInfo.descriptorPool = *descriptorPool;
            allocInfo.descriptorSetCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
            allocInfo.pSetLayouts = layouts.data();

            descriptorSets->resize(MAX_FRAMES_IN_FLIGHT);

            if (vkAllocateDescriptorSets(*device, &allocInfo, descriptorSets->data()) != VK_SUCCESS)
            {
                throw std::runtime_error("failed to allocate descriptor sets!");
            }

            for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
            {
                DynamicBuffer uniformBuffer = (*uniformBuffers)[i];
                VkBuffer vkUniformBuffer = uniformBuffer.getVulkanBuffer();

                VkDescriptorBufferInfo bufferInfo{};
                bufferInfo.buffer = vkUniformBuffer;
                bufferInfo.offset = 0;
                bufferInfo.range = sizeof(uniformBufferSize);

                VkDescriptorImageInfo imageInfo{};
                imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
                imageInfo.imageView = *textureImageView;
                imageInfo.sampler = *textureSampler;

                std::array<VkWriteDescriptorSet, 2> descriptorWrites{};

                descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                descriptorWrites[0].dstSet = (*descriptorSets)[i];
                descriptorWrites[0].dstBinding = 0;
                descriptorWrites[0].dstArrayElement = 0;
                descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
                descriptorWrites[0].descriptorCount = 1;
                descriptorWrites[0].pBufferInfo = &bufferInfo;

                descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                descriptorWrites[1].dstSet = (*descriptorSets)[i];
                descriptorWrites[1].dstBinding = 1;
                descriptorWrites[1].dstArrayElement = 0;
                descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
                descriptorWrites[1].descriptorCount = 1;
                descriptorWrites[1].pImageInfo = &imageInfo;

                vkUpdateDescriptorSets(*device, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
            }
        }


    }
}