#include<array>
#include<stdexcept>
#include<vector>

#include"../../Include/Vulkan/VulkanDescriptor.h"

namespace DRHI
{
    namespace VulkanDescriptor
    {
        //-------------------------------------------------------------
        //-------------------------------------------------------------
        //----------------------private function-----------------------
        //-------------------------------------------------------------
        //-------------------------------------------------------------
        VkWriteDescriptorSet createWriteDescriptorSets(VkDescriptorSet* descriptorSet, VkDescriptorBufferInfo bufferInfo, VkDevice* device, VkImageView* textureImageView, VkSampler* textureSampler)
        {
            VkDescriptorImageInfo imageInfo{};
            imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            imageInfo.imageView = *textureImageView;
            imageInfo.sampler = *textureSampler;

            VkWriteDescriptorSet writeDescriptorSet{};
            writeDescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            writeDescriptorSet.dstSet = *descriptorSet;
            writeDescriptorSet.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            writeDescriptorSet.dstBinding = 0;
            writeDescriptorSet.pBufferInfo = &bufferInfo;
            writeDescriptorSet.descriptorCount = 1;
            writeDescriptorSet.pImageInfo = &imageInfo;

            return writeDescriptorSet;
        }




        //-------------------------------------------------------------
        //-------------------------------------------------------------
        //-----------------------public function-----------------------
        //-------------------------------------------------------------
        //-------------------------------------------------------------
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

            if (vkCreateDescriptorSetLayout(*device, &layoutInfo, nullptr, descriptorSetLayout) != VK_SUCCESS) 
            {
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

            if (vkCreateDescriptorPool(*device, &poolInfo, nullptr, descriptorPool) != VK_SUCCESS)
            {
                throw std::runtime_error("failed to create descriptor pool!");
            }
        }

        void createDescriptorSet(VkDescriptorSet* descriptorSet, VkDescriptorPool* descriptorPool, VkDescriptorSetLayout* descriptorSetLayout, uint32_t descriptorSetCount, VkDevice* device, std::vector<DynamicDescriptorBufferInfo>* uniformBufferInfo, VkImageView* textureImageView, VkSampler* textureSampler)
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

            DynamicWriteDescriptorSet wd{};

            VkWriteDescriptorSet vkwd{};
            vkwd.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            vkwd.dstSet = wd.dstSet.getVulkanDescriptorSet();
            vkwd.descriptorType = (VkDescriptorType)wd.descriptorType;
            vkwd.dstBinding = wd.dstBinding;
            vkwd.pBufferInfo = &wd.pBufferInfo->getVulkanDescriptorBufferInfo();

            auto buffer1 = (*uniformBufferInfo)[0].getVulkanDescriptorBufferInfo();
            VkWriteDescriptorSet writeDescriptorSet1{};
            writeDescriptorSet1.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            writeDescriptorSet1.dstSet = *descriptorSet;
            writeDescriptorSet1.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            writeDescriptorSet1.dstBinding = 0;
            writeDescriptorSet1.pBufferInfo = &buffer1;
            writeDescriptorSet1.descriptorCount = 1;

            VkDescriptorImageInfo imageInfo{};
            imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            imageInfo.imageView = *textureImageView;
            imageInfo.sampler = *textureSampler;

            auto buffer2 = (*uniformBufferInfo)[1].getVulkanDescriptorBufferInfo();
            VkWriteDescriptorSet writeDescriptorSet2{};
            writeDescriptorSet2.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            writeDescriptorSet2.dstSet = *descriptorSet;
            writeDescriptorSet2.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
            writeDescriptorSet2.dstBinding = 1;
            writeDescriptorSet2.pBufferInfo = &buffer2;
            writeDescriptorSet2.descriptorCount = 1;
            writeDescriptorSet2.pImageInfo = &imageInfo;

            std::vector<VkWriteDescriptorSet> writeDescriptorSets{ writeDescriptorSet1, writeDescriptorSet2 };

            vkUpdateDescriptorSets(*device, static_cast<uint32_t>(writeDescriptorSets.size()), writeDescriptorSets.data(), 0, nullptr);
        }
    }
}