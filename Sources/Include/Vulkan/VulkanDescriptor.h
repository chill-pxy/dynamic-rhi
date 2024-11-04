#include<vector>
#include "../InterfaceType.h"

namespace DRHI
{
    namespace VulkanDescriptor
    {
        void createDescriptorSetLayout(VkDescriptorSetLayout* descriptorSetLayout, std::vector<DynamicDescriptorSetLayoutBinding>* dsbs, VkDevice* device);
        void createDescriptorPool(VkDescriptorPool* descriptorPool, std::vector<VkDescriptorPoolSize>* poolSizes, VkDevice* device);
        void createDescriptorPool(VkDescriptorPool* descriptorPool, VkDevice* device);
        void createDescriptorSet(VkDescriptorSet* descriptorSet, VkDescriptorPool* descriptorPool, VkDescriptorSetLayout* descriptorSetLayout, std::vector<DynamicWriteDescriptorSet>* wds, VkDevice* device);
    }
}