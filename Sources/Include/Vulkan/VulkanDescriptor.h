#include<vector>
#include "../InterfaceType.h"

namespace DRHI
{
    namespace VulkanDescriptor
    {
        void createDescriptorSetLayout(VkDescriptorSetLayout* descriptorSetLayout, VkDevice* device);
        void createDescriptorPool(VkDescriptorPool* descriptorPool, VkDevice* device);
        void createDescriptorSet(VkDescriptorSet* descriptorSet, VkDescriptorPool* descriptorPool, VkDescriptorSetLayout* descriptorSetLayout, uint32_t descriptorSetCount, VkDevice* device, std::vector<DynamicDescriptorBufferInfo>* uniformBufferInfo,VkImageView* textureImageView, VkSampler* textureSampler);
    }
}