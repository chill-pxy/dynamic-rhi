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
        void createDescriptorSetLayout(VkDescriptorSetLayout* descriptorSetLayout, std::vector<DynamicDescriptorSetLayoutBinding>* dsbs, VkDevice* device)
        {
            std::vector<VkDescriptorSetLayoutBinding> descriptorSetLayoutBinding;

            for (int i = 0; i < dsbs->size(); ++i)
            {
                DynamicDescriptorSetLayoutBinding dsb = (*dsbs)[i];

                VkDescriptorSetLayoutBinding vkdsb{};
                vkdsb.binding = dsb.binding;
                vkdsb.descriptorCount = dsb.descriptorCount;
                vkdsb.descriptorType = (VkDescriptorType)dsb.descriptorType;
                vkdsb.stageFlags = (VkShaderStageFlags)dsb.stageFlags;

                if (dsb.pImmutableSamplers != nullptr)
                {
                    VkSampler vksampler = dsb.pImmutableSamplers->getVulkanSampler();
                    vkdsb.pImmutableSamplers = &vksampler;
                }

                descriptorSetLayoutBinding.push_back(vkdsb);
            }

            VkDescriptorSetLayoutCreateInfo layoutInfo{};
            layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
            layoutInfo.bindingCount = static_cast<uint32_t>(descriptorSetLayoutBinding.size());
            layoutInfo.pBindings = descriptorSetLayoutBinding.data();

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

        void createDescriptorSet(VkDescriptorSet* descriptorSet, VkDescriptorPool* descriptorPool, VkDescriptorSetLayout* descriptorSetLayout, std::vector<DynamicWriteDescriptorSet>* wds, VkDevice* device)
        {
            VkDescriptorSetAllocateInfo descriptorSetAllocateInfo{};
            descriptorSetAllocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
            descriptorSetAllocateInfo.descriptorPool = *descriptorPool;
            descriptorSetAllocateInfo.pSetLayouts = descriptorSetLayout;
            descriptorSetAllocateInfo.descriptorSetCount = 1;
            if (vkAllocateDescriptorSets(*device, &descriptorSetAllocateInfo, descriptorSet))
            {
                throw std::runtime_error("failed to allocate descriptorsets");
            }

            std::vector<VkWriteDescriptorSet> writeDescriptorSets;

            for (int i = 0; i < wds->size(); ++i)
            {
                DynamicWriteDescriptorSet wd = (*wds)[i];

                VkWriteDescriptorSet vkwd{};
                vkwd.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                vkwd.dstSet = *descriptorSet;
                vkwd.descriptorType = (VkDescriptorType)wd.descriptorType;
                vkwd.descriptorCount = wd.descriptorCount;
                vkwd.dstBinding = wd.dstBinding;

                if (wd.pBufferInfo != nullptr)
                {
                    auto bufferInfo = wd.pBufferInfo->getVulkanDescriptorBufferInfo();
                    vkwd.pBufferInfo = &bufferInfo;
                }

                if (wd.pImageInfo != nullptr)
                {
                    VkDescriptorImageInfo vkdescriptorImageInfo{};
                    vkdescriptorImageInfo.sampler = wd.pImageInfo->sampler.getVulkanSampler();
                    vkdescriptorImageInfo.imageView = wd.pImageInfo->imageView.getVulkanImageView();
                    vkdescriptorImageInfo.imageLayout = (VkImageLayout)wd.pImageInfo->imageLayout;
                    vkwd.pImageInfo = &vkdescriptorImageInfo;
                }

                writeDescriptorSets.push_back(vkwd);
            }

            vkUpdateDescriptorSets(*device, static_cast<uint32_t>(writeDescriptorSets.size()), writeDescriptorSets.data(), 0, nullptr);
        }
    }
}