#include "../InterfaceType.h"

namespace DRHI
{
    void createDescriptorSetLayout(VkDescriptorSetLayout* descriptorSetLayout, VkDevice* device);
    void createDescriptorPool(VkDescriptorPool* descriptorPool, VkDevice* device);
    void createDescriptorSet(VkDescriptorSet* descriptorSet, VkDescriptorPool* descriptorPool, VkDescriptorSetLayout* descriptorSetLayout, uint32_t descriptorSetCount, VkDevice* device);
}