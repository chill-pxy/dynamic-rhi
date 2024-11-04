#include"../../Include/Vulkan/VulkanDRHI.h"
#include"../../Include/Vulkan/VulkanShader.h"
#include"../../Include/CoreFunction.h"

namespace DRHI
{
    //-------------------------------------command functions------------------------------------
    void VulkanDRHI::beginCommandBuffer(uint32_t index)
    {
        VkCommandBufferBeginInfo cmdBufferBeginInfo{};
        cmdBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        vkBeginCommandBuffer(_commandBuffers[index], &cmdBufferBeginInfo);

        // With dynamic rendering there are no subpass dependencies, so we need to take care of proper layout transitions by using barriers
        // This set of barriers prepares the color and depth images for output
        insertImageMemoryBarrier(_commandBuffers[index], _swapChainImages[index], 0,
            VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
            VK_IMAGE_LAYOUT_UNDEFINED,
            VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
            VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
            VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
            VkImageSubresourceRange{ VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 });

        ////need to setup depth image
        insertImageMemoryBarrier(_commandBuffers[index], _depthStencil.image, 0,
            VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT,
            VK_IMAGE_LAYOUT_UNDEFINED,
            VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
            VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT,
            VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT,
            VkImageSubresourceRange{ VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT, 0, 1, 0, 1 });

        // New structures are used to define the attachments used in dynamic rendering
        VkRenderingAttachmentInfoKHR colorAttachment{};
        colorAttachment.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO_KHR;
        colorAttachment.imageView = _swapChainImageViews[index];
        colorAttachment.imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        colorAttachment.clearValue.color = { 0.52f, 0.52f, 0.52f,0.0f };

        // A single depth stencil attachment info can be used, but they can also be specified separately.
        // When both are specified separately, the only requirement is that the image view is identical.			
        VkRenderingAttachmentInfoKHR depthStencilAttachment{};
        depthStencilAttachment.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO_KHR;
        depthStencilAttachment.imageView = _depthStencil.view;
        depthStencilAttachment.imageLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
        depthStencilAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        depthStencilAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        depthStencilAttachment.clearValue.depthStencil = { 1.0f,  0 };

        VkRenderingInfoKHR renderingInfo{};
        renderingInfo.sType = VK_STRUCTURE_TYPE_RENDERING_INFO_KHR;
        renderingInfo.renderArea = { 0, 0, _viewPortWidth, _viewPortHeight };
        renderingInfo.layerCount = 1;
        renderingInfo.colorAttachmentCount = 1;
        renderingInfo.pColorAttachments = &colorAttachment;
        renderingInfo.pDepthAttachment = &depthStencilAttachment;
        renderingInfo.pStencilAttachment = &depthStencilAttachment;

        //Begin dynamic rendering
        vkCmdBeginRenderingKHR(_commandBuffers[index], &renderingInfo);

        VkViewport viewport{};
        viewport.width = _viewPortWidth;
        viewport.height = _viewPortHeight;
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;

        vkCmdSetViewport(_commandBuffers[index], 0, 1, &viewport);

        VkRect2D scissor{};
        scissor.extent.width = _viewPortWidth;
        scissor.extent.height = _viewPortHeight;
        scissor.offset.x = 0;
        scissor.offset.y = 0;

        vkCmdSetScissor(_commandBuffers[index], 0, 1, &scissor);
    }

    uint32_t VulkanDRHI::getCommandBufferSize()
    {
        return _commandBuffers.size();
    }

    void VulkanDRHI::endCommandBuffer(uint32_t index)
    {
        vkCmdEndRenderingKHR(_commandBuffers[index]);

        insertImageMemoryBarrier(_commandBuffers[index], _swapChainImages[index],
            VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
            0,
            VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
            VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
            VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
            VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,
            VkImageSubresourceRange{ VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 });

        vkEndCommandBuffer(_commandBuffers[index]);
    }
    //----------------------------------------------------------------------------------------





    //-------------------------------------buffer functions-----------------------------------
    void VulkanDRHI::bindVertexBuffers(DynamicBuffer* vertexBuffer, uint32_t index)
    {
        auto vkVertexBuffer = vertexBuffer->getVulkanBuffer();
        const VkDeviceSize offsets[1] = { 0 };
        vkCmdBindVertexBuffers(_commandBuffers[index], 0, 1, &vkVertexBuffer, offsets);
    }

    void VulkanDRHI::bindIndexBuffer(DynamicBuffer* indexBuffer, uint32_t index, uint32_t indexType)
    {
        auto vkIndexBuffer = indexBuffer->getVulkanBuffer();
        vkCmdBindIndexBuffer(_commandBuffers[index], vkIndexBuffer, 0, (VkIndexType)indexType);
    }

    void VulkanDRHI::createDynamicBuffer(DynamicBuffer* buffer, DynamicDeviceMemory* deviceMemory, uint64_t bufferSize, void* bufferData, uint32_t usage, uint32_t memoryProperty)
    {
        VulkanBuffer::createDynamicBuffer(buffer, deviceMemory, bufferSize, bufferData, &_device, &_physicalDevice, &_commandPool, &_graphicQueue, usage, memoryProperty);
    }

    void VulkanDRHI::createUniformBuffer(std::vector<DynamicBuffer>* uniformBuffers, std::vector<DynamicDeviceMemory>* uniformBuffersMemory, std::vector<void*>* uniformBuffersMapped, uint32_t bufferSize)
    {
        uniformBuffers->resize(bufferSize);
        uniformBuffersMemory->resize(bufferSize);
        uniformBuffersMapped->resize(bufferSize);

        VkBuffer vkUniformBuffer;
        VkDeviceMemory vkUniformBufferMemory;

        VulkanBuffer::createBuffer(&_device, &_physicalDevice, bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, &vkUniformBuffer, &vkUniformBufferMemory);

        for (size_t i = 0; i < bufferSize; i++)
        {
            vkMapMemory(_device, vkUniformBufferMemory, 0, bufferSize, 0, &(*uniformBuffersMapped)[i]);

            (*uniformBuffers)[i].internalID = vkUniformBuffer;
            (*uniformBuffersMemory)[i].internalID = vkUniformBufferMemory;
        }
    }

    uint32_t VulkanDRHI::getCurrentBuffer()
    {
        return _currentBuffer;
    }

    void VulkanDRHI::clearBuffer(DynamicBuffer* buffer, DynamicDeviceMemory* memory)
    {
        vkDestroyBuffer(_device, std::get<VkBuffer>(buffer->internalID), nullptr);
        vkFreeMemory(_device, std::get<VkDeviceMemory>(memory->internalID), nullptr);
    }

    void VulkanDRHI::flushBuffer(DynamicDeviceMemory* memory, uint32_t size, uint32_t offset)
    {
        auto vkmemory = memory->getVulkanDeviceMemory();
        VkMappedMemoryRange mappedRange = {};
        mappedRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
        mappedRange.memory = vkmemory;
        mappedRange.offset = offset;
        mappedRange.size = size;
        vkFlushMappedMemoryRanges(_device, 1, &mappedRange);
    }

    void VulkanDRHI::flushBuffer(DynamicDeviceMemory* memory, uint32_t offset)
    {
        auto vkmemory = memory->getVulkanDeviceMemory();
        VkMappedMemoryRange mappedRange = {};
        mappedRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
        mappedRange.memory = vkmemory;
        mappedRange.offset = offset;
        mappedRange.size = VK_WHOLE_SIZE;
        vkFlushMappedMemoryRanges(_device, 1, &mappedRange);
    }
    //----------------------------------------------------------------------------------------






    //------------------------------------pipeline functions----------------------------------
    void VulkanDRHI::createPipelineLayout(DynamicPipelineLayout* pipelineLayout, DynamicPipelineLayoutCreateInfo* createInfo)
    {
        VulkanPipeline::createPipelineLayout(pipelineLayout, createInfo, &_device);
    }

    void VulkanDRHI::createPipeline(DynamicPipeline* pipeline, DynamicPipelineLayout* pipelineLayout, DynamicPipelineCreateInfo info)
    {
        VkPipeline vkpipeline;
        VkPipelineLayout vkpipelineLayout = pipelineLayout->getVulkanPipelineLayout();

        auto vertex = readFile(info.vertexShader);
        auto fragment = readFile(info.fragmentShader);

        auto vulkanVertex = createShaderModule(vertex, &_device);
        auto vulkanFragment = createShaderModule(fragment, &_device);

        VulkanPipeline::VulkanPipelineCreateInfo pci{};
        pci.vertexShader = vulkanVertex;
        pci.fragmentShader = vulkanFragment;

        auto vkVertexInputBinding = info.vertexInputBinding.getVulkanVertexInputBindingDescription();

        std::vector<VkVertexInputAttributeDescription> vkVertexInputAttribute;

        for (int i = 0; i < info.vertexInputAttributes.size(); ++i)
        {
            vkVertexInputAttribute.emplace_back(info.vertexInputAttributes[i].getVulkanVertexInputAttributeDescription());
        }

        VulkanPipeline::createGraphicsPipeline(&vkpipeline, &vkpipelineLayout, &_pipelineCache, pci, &_device, &_swapChainImageFormat, vkVertexInputBinding, vkVertexInputAttribute);

        pipeline->internalID = vkpipeline;
    }

    void VulkanDRHI::bindPipeline(DynamicPipeline pipeline, uint32_t bindPoint, uint32_t index)
    {
        vkCmdBindPipeline(_commandBuffers[index], (VkPipelineBindPoint)bindPoint, pipeline.getVulkanPipeline());
    }

    VkPipelineRenderingCreateInfoKHR VulkanDRHI::getPipelineRenderingCreateInfo()
    {
        return VulkanPipeline::getPipelineRenderingCreateInfo(&_swapChainImageFormat);
    }
    //----------------------------------------------------------------------------------------








    //------------------------------------memory functions------------------------------------
    void VulkanDRHI::mapMemory(DynamicDeviceMemory* memory, uint32_t offset, uint32_t size, void* data)
    {
        auto vkmemory = memory->getVulkanDeviceMemory();
        vkMapMemory(_device, vkmemory, offset, size, 0, &data);
    }

    void VulkanDRHI::unmapMemory(DynamicDeviceMemory* memory)
    {
        auto vkmemory = memory->getVulkanDeviceMemory();
        vkUnmapMemory(_device, vkmemory);
    }
    //----------------------------------------------------------------------------------------








    //-------------------------------------descriptor functions-------------------------------
    void VulkanDRHI::createDescriptorPool(DynamicDescriptorPool* descriptorPool, std::vector<DynamicDescriptorPoolSize>* poolsizes)
    {
        std::vector<VkDescriptorPoolSize> vkpoolSizes{};

        for (int i = 0; i < poolsizes->size(); ++i)
        {
            auto vkpoolsize = VkDescriptorPoolSize();
            vkpoolsize.descriptorCount = (*poolsizes)[i].descriptorCount;
            vkpoolsize.type = (VkDescriptorType)(*poolsizes)[i].type;
            vkpoolSizes.push_back(vkpoolsize);
        }

        VkDescriptorPool vkdescriptorPool{};
        VulkanDescriptor::createDescriptorPool(&vkdescriptorPool, &vkpoolSizes, &_device);
        descriptorPool->internalID = vkdescriptorPool;
    }

    void VulkanDRHI::createDescriptorPool(DynamicDescriptorPool* descriptorPool)
    {
        VkDescriptorPool vkdescriptorPool{};
        VulkanDescriptor::createDescriptorPool(&vkdescriptorPool, &_device);
        descriptorPool->internalID = vkdescriptorPool;
    }
    
    void VulkanDRHI::createDescriptorSet(DynamicDescriptorSet* descriptorSet, DynamicDescriptorSetLayout* descriptorSetLayout, DynamicDescriptorPool* descriptorPool, std::vector<DynamicWriteDescriptorSet>* wds)
    {
        VkDescriptorSet vkdescriptorSet{};
        VkDescriptorPool vkdescriptorPool = descriptorPool->getVulkanDescriptorPool();
        VkDescriptorSetLayout vkdescriptorSetLayout = descriptorSetLayout->getVulkanDescriptorSetLayout();
        VulkanDescriptor::createDescriptorSet(&vkdescriptorSet, &vkdescriptorPool, &vkdescriptorSetLayout, wds, &_device);
        descriptorSet->internalID = vkdescriptorSet;
        descriptorSetLayout->internalID = vkdescriptorSetLayout;
    }

    void VulkanDRHI::bindDescriptorSets(DynamicDescriptorSet* descriptorSet, DynamicPipelineLayout pipelineLayout, uint32_t bindPoint, uint32_t index)
    {
        auto vkDescriptorSet = descriptorSet->getVulkanDescriptorSet();
        vkCmdBindDescriptorSets(_commandBuffers[index], (VkPipelineBindPoint)bindPoint, pipelineLayout.getVulkanPipelineLayout(), 0, 1, &vkDescriptorSet, 0, nullptr);
        descriptorSet->internalID = vkDescriptorSet;
    }

    void VulkanDRHI::createDescriptorSetLayout(DynamicDescriptorSetLayout* descriptorSetLayout, std::vector<DynamicDescriptorSetLayoutBinding>* dsbs)
    {
        VkDescriptorSetLayout vkdescriptorSetLayout{};

        VulkanDescriptor::createDescriptorSetLayout(&vkdescriptorSetLayout, dsbs, &_device);

        descriptorSetLayout->internalID = vkdescriptorSetLayout;
    }
    //-----------------------------------------------------------------------------------------






    //------------------------------------texture functions-------------------------------------
    void VulkanDRHI::createTextureImage(DynamicImage* textureImage, DynamicDeviceMemory* textureMemory, int texWidth, int texHeight, int texChannels, stbi_uc* pixels)
    {
        VkImage vkImage;
        VkDeviceMemory vkMemory;
        VulkanImage::createTextureImage(&vkImage, &vkMemory, texWidth, texHeight, texChannels, pixels, &_device, &_physicalDevice, &_graphicQueue, &_commandPool);
        textureImage->internalID = vkImage;
        textureMemory->internalID = vkMemory;
    }

    void VulkanDRHI::createTextureSampler(DynamicSampler* textureSampler)
    {
        VkSampler vkSampler;
        VulkanImage::createTextureSampler(&vkSampler, &_physicalDevice, &_device);
        textureSampler->internalID = vkSampler;
    }
    //------------------------------------------------------------------------------------------





    //-----------------------------------image functions----------------------------------------
    void VulkanDRHI::createImageView(DynamicImageView* imageView, DynamicImage* image, uint32_t imageFormat)
    {
        VkImage vkImage = image->getVulkanImage();
        VkImageView vkTextureImageView = VulkanImage::createImageView(&_device, &vkImage, (VkFormat)imageFormat, VK_IMAGE_ASPECT_COLOR_BIT);
        imageView->internalID = vkTextureImageView;
    }

    void VulkanDRHI::createImage(DynamicImage* image, uint32_t width, uint32_t height, 
        uint32_t format, uint32_t imageTiling, uint32_t imageUsageFlagBits, uint32_t memoryPropertyFlags, DynamicDeviceMemory* imageMemory)
    {
        VkImage vkimage{};
        VkDeviceMemory vkmemory{};
        VkFormat vkformat = (VkFormat)format;
        VkImageTiling vkimageTiling = (VkImageTiling)imageTiling;
        VkImageUsageFlagBits vkImageUsageFlagBits = (VkImageUsageFlagBits)imageUsageFlagBits;
        VkMemoryPropertyFlagBits vkMemoryPropertyFlagBits = (VkMemoryPropertyFlagBits)memoryPropertyFlags;

        VulkanImage::createImage(&vkimage, width, height, vkformat, vkimageTiling, vkImageUsageFlagBits, vkMemoryPropertyFlagBits, vkmemory, &_device, &_physicalDevice);

        image->internalID = vkimage;
        imageMemory->internalID = vkmemory;
    }

    void VulkanDRHI::copyBufferToImage(DynamicBuffer* buffer, DynamicImage* image, uint32_t width, uint32_t height)
    {
        VkBuffer vkbuffer = buffer->getVulkanBuffer();
        VkImage vkimage = image->getVulkanImage();
        
        VulkanImage::copyBufferToImage(&vkbuffer, &vkimage, width, height, &_graphicQueue, &_commandPool, &_device);
        
        buffer->internalID = vkbuffer;
        image->internalID = vkimage;
    }

    void VulkanDRHI::createSampler(DynamicSampler* sampler, DynamicSmplerCreateInfo createInfo)
    {
        VkSampler vksampler{};
        VulkanImage::createSampler(&vksampler, createInfo, &_physicalDevice, &_device);
        sampler->internalID = vksampler;
    }

    void VulkanDRHI::clearImage(DynamicSampler* sampler, DynamicImageView* imageView, DynamicImage* image, DynamicDeviceMemory* memory)
    {
        vkDestroySampler(_device, std::get<VkSampler>(sampler->internalID), nullptr);
        vkDestroyImageView(_device, std::get<VkImageView>(imageView->internalID), nullptr);

        vkDestroyImage(_device, std::get<VkImage>(image->internalID), nullptr);
        vkFreeMemory(_device, std::get<VkDeviceMemory>(memory->internalID), nullptr);
    }
    //-----------------------------------------------------------------------------------------------






    //------------------------------------ cmd functions --------------------------------------------
    void VulkanDRHI::cmdPushConstants(uint32_t index, DynamicPipelineLayout* layout, uint32_t stage, uint32_t offset, uint32_t size, void* value)
    {
        auto vklayout = layout->getVulkanPipelineLayout();
        vkCmdPushConstants(_commandBuffers[index], vklayout, (VkShaderStageFlags)stage, offset, size, value);
    }
    //-----------------------------------------------------------------------------------------------
}