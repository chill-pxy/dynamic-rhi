#include<vector>
#include "../InterfaceType.h"

namespace DRHI
{
    namespace VulkanDescriptor
    {
        void createDescriptorSetLayout(VkDescriptorSetLayout* descriptorSetLayout, VkDevice* device);
        void createDescriptorPool(VkDescriptorPool* descriptorPool, VkDevice* device);
        void createDescriptorSet(VkDescriptorSet* descriptorSet, VkDescriptorPool* descriptorPool, VkDescriptorSetLayout* descriptorSetLayout, uint32_t descriptorSetCount, VkDevice* device, std::vector<DynamicDescriptorBufferInfo>* uniformBufferInfo);
        void createDescriptorSets(std::vector<VkDescriptorSet>* descriptorSets, VkDescriptorSetLayout* descriptorSetLayout, VkDescriptorPool* descriptorPool, VkDevice* device, std::vector<DynamicBuffer>* uniformBuffers, uint32_t uniformBufferSize, VkImageView* textureImageView, VkSampler* textureSampler);
    }
}