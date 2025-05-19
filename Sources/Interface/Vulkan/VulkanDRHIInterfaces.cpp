#include"../../Include/Vulkan/VulkanDRHI.h"
#include"../../Include/Vulkan/VulkanShader.h"
#include"../../Include/CoreFunction.h"

namespace drhi
{
    //-------------------------------------   swap chain    ------------------------------------
    uint32_t VulkanDRHI::getSwapChainExtentWidth()
    {
        return _swapChainExtent.width;
    }

    uint32_t VulkanDRHI::getSwapChainExtentHeight()
    {
        return _swapChainExtent.height;
    }

    //-------------------------------------command functions------------------------------------
    void VulkanDRHI::createCommandPool(DynamicCommandPool* commandPool)
    {
        VkCommandPool vkcommandPool{};
        VulkanCommand::createCommandPool(&vkcommandPool, &_device, _queueFamilyIndices);
        commandPool->internalID = vkcommandPool;
    }
    
    void VulkanDRHI::createCommandBuffers(std::vector<DynamicCommandBuffer>* commandBuffers, DynamicCommandPool* commandPool, DynamicCommandBufferLevel level)
    {
        std::vector<VkCommandBuffer> vkcommandbuffers{};
        auto vkcommandPool = commandPool->getVulkanCommandPool();
        auto vklevel = VkCommandBufferLevel::VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        if (level == DynamicCommandBufferLevel::SECONDARY) vklevel = VkCommandBufferLevel::VK_COMMAND_BUFFER_LEVEL_SECONDARY;

        VulkanCommand::createCommandBuffers(&vkcommandbuffers, &vkcommandPool, vklevel, &_device);
        commandPool->internalID = vkcommandPool;

        commandBuffers->resize(vkcommandbuffers.size());
        for (uint32_t i = 0; i < vkcommandbuffers.size(); ++i)
        {
            (*commandBuffers)[i].internalID = vkcommandbuffers[i];
        }
    }

    void VulkanDRHI::createCommandBuffer(DynamicCommandBuffer* commandBuffer, DynamicCommandPool* commandPool, DynamicCommandBufferLevel level)
    {
        auto vklevel = VkCommandBufferLevel::VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        if (level == DynamicCommandBufferLevel::SECONDARY) vklevel = VkCommandBufferLevel::VK_COMMAND_BUFFER_LEVEL_SECONDARY;

        VkCommandBufferAllocateInfo info{};
        info.commandBufferCount = 1;
        info.commandPool = commandPool->getVulkanCommandPool();
        info.level = vklevel;
        info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;

        VkCommandBuffer cmb{};
        vkAllocateCommandBuffers(_device, &info, &cmb);

        commandBuffer->internalID = cmb;
    }

    void VulkanDRHI::beginCommandBuffer(DynamicCommandBuffer commandBuffer)
    {
        VkCommandBuffer vkCommandBuffer = commandBuffer.getVulkanCommandBuffer();

        VkCommandBufferBeginInfo cmdBufferBeginInfo{};
        cmdBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        vkBeginCommandBuffer(vkCommandBuffer, &cmdBufferBeginInfo);
    }

    void VulkanDRHI::beginRendering(DynamicCommandBuffer commandBuffer, DynamicRenderingInfo bri)
    {
        VkCommandBuffer vkCommandBuffer = commandBuffer.getVulkanCommandBuffer();
        VkImage vkImage{};
        VkImageView vkImageView{};
        VkImage vkDepthImage{};
        VkImageView vkDepthImageView{};

        uint32_t width = 0, height = 0;
        
        if (bri.isRenderOnSwapChain)
        {
            vkImage = _swapChainImages[bri.swapChainIndex];
            vkImageView = _swapChainImageViews[bri.swapChainIndex];
            vkDepthImage = _depthStencil.image;
            vkDepthImageView = _depthStencil.view;
            VulkanCommand::insertImageMemoryBarrier(&vkCommandBuffer, &vkImage,
                0,
                VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
                VK_IMAGE_LAYOUT_UNDEFINED,
                VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
                VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
                VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
                VkImageSubresourceRange{ VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 });

            VulkanCommand::insertImageMemoryBarrier(&vkCommandBuffer, &vkDepthImage,
                0,
                VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT,
                VK_IMAGE_LAYOUT_UNDEFINED,
                (VkImageLayout)bri.depthImageLayout,//VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
                VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT,
                VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT,
                VkImageSubresourceRange{ VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT, 0, 1, 0, 1 });

            width = _swapChainExtent.width;
            height = _swapChainExtent.height;
        }
        else
        {
            if (bri.targetImage->valid() && bri.targetImageView->valid())
            {
                vkImage = bri.targetImage->getVulkanImage();
                vkImageView = bri.targetImageView->getVulkanImageView();
                VulkanCommand::insertImageMemoryBarrier(&vkCommandBuffer, &vkImage,
                    0,
                    VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
                    VK_IMAGE_LAYOUT_UNDEFINED,
                    VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
                    VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
                    VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
                    VkImageSubresourceRange{ (VkImageAspectFlags)bri.colorAspectFlag, 0, 1, 0, 1 });
            }
          
            if (bri.targetDepthImage->valid() && bri.targetDepthImageView->valid())
            {
                vkDepthImage = bri.targetDepthImage->getVulkanImage();
                vkDepthImageView = bri.targetDepthImageView->getVulkanImageView();

                VulkanCommand::insertImageMemoryBarrier(&vkCommandBuffer, &vkDepthImage,
                    0,
                    VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT,
                    VK_IMAGE_LAYOUT_UNDEFINED,
                    (VkImageLayout)bri.depthImageLayout,
                    VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT,
                    VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT,
                    VkImageSubresourceRange{ (VkImageAspectFlags)bri.depthAspectFlag, 0, 1, 0, 1 });
            } 

            width = bri.targetImageWidth; //_swapChainExtent.width;
            height = bri.targetImageHeight; //_swapChainExtent.height;
        }   

        VulkanCommand::beginRendering(vkCommandBuffer, &vkImage, &vkDepthImage, &vkImageView, &vkDepthImageView, width, height, bri.isClearEveryFrame, bri.includeStencil);
    
        if (bri.isRenderOnSwapChain)
        {
           _swapChainImages[bri.swapChainIndex] = vkImage;
           _swapChainImageViews[bri.swapChainIndex] = vkImageView;
        }
        else
        {
            if (bri.targetImage->valid() && bri.targetImageView->valid())
            {
                bri.targetImage->internalID = vkImage;
                bri.targetImageView->internalID = vkImageView;
            }
            
            if (bri.targetDepthImage->valid() && bri.targetDepthImageView->valid())
            {
                bri.targetDepthImage->internalID = vkDepthImage;
                bri.targetDepthImageView->internalID = vkDepthImageView;
            }
        }
    }

    void VulkanDRHI::endCommandBuffer(DynamicCommandBuffer commandBuffer)
    {
        VkCommandBuffer vkCommandBuffer = commandBuffer.getVulkanCommandBuffer();
        vkEndCommandBuffer(vkCommandBuffer);
    }

    void VulkanDRHI::endRendering(DynamicCommandBuffer commandBuffer, DynamicRenderingInfo bri)
    {
        VkCommandBuffer vkCommandBuffer = commandBuffer.getVulkanCommandBuffer();
        vkCmdEndRendering(vkCommandBuffer);

        VkImage vkImage{};
        VkImage vkdepthImage{};

        if (bri.isRenderOnSwapChain)
        {
            vkImage = _swapChainImages[bri.swapChainIndex];   
            VulkanCommand::insertImageMemoryBarrier(&vkCommandBuffer, &vkImage,
                VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
                0,
                VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
                VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
                VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
                VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,
                VkImageSubresourceRange{ VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 });
        }
        else
        {
            if (bri.targetImage->valid())
            {
                vkImage = bri.targetImage->getVulkanImage();
                VulkanCommand::insertImageMemoryBarrier(&vkCommandBuffer, &vkImage,
                    VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
                    0,
                    VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
                    VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
                    VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
                    VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,
                    VkImageSubresourceRange{ (VkImageAspectFlags)bri.colorAspectFlag, 0, 1, 0, 1 });
            }
        }

        commandBuffer.internalID = vkCommandBuffer;

        if (bri.isRenderOnSwapChain)
        {
            _swapChainImages[bri.swapChainIndex] = vkImage;
        }
        else
        {
            if (bri.targetImage->valid())
            {
                bri.targetImage->internalID = vkImage;
            }
        }
    }

    void VulkanDRHI::freeCommandBuffers(std::vector<DynamicCommandBuffer>* commandBuffers, DynamicCommandPool* commandPool)
    {
        auto vkcommandPool = commandPool->getVulkanCommandPool();

        std::vector<VkCommandBuffer> vkcommandBuffers{};
        for (uint32_t i = 0; i < commandBuffers->size(); ++i)
        {
            vkcommandBuffers.push_back((*commandBuffers)[i].getVulkanCommandBuffer());
        }

        vkFreeCommandBuffers(_device, vkcommandPool, commandBuffers->size(), vkcommandBuffers.data());
    }

    void VulkanDRHI::freeCommandBuffer(DynamicCommandBuffer* commandBuffer, DynamicCommandPool* commandPool)
    {
        auto vkcommandPool = commandPool->getVulkanCommandPool();
        auto vkcmdbuf = commandBuffer->getVulkanCommandBuffer();

        vkFreeCommandBuffers(_device, vkcommandPool, 1, &vkcmdbuf);
    }

    void VulkanDRHI::destroyCommandPool(DynamicCommandPool* commandPool)
    {
        auto vkcommandPool = commandPool->getVulkanCommandPool();
        vkDestroyCommandPool(_device, vkcommandPool, nullptr);
        commandPool->internalID = nullptr;
    }

    void VulkanDRHI::flushCommandBuffer(DynamicCommandBuffer cmdBuf, DynamicCommandPool cmdPool, bool free)
    {
        VulkanCommand::flushCommandBuffer(_device, cmdBuf.getVulkanCommandBuffer(), _graphicQueue, cmdPool.getVulkanCommandPool(), free);
    }
    //----------------------------------------------------------------------------------------





    //-------------------------------------buffer functions-----------------------------------
    void VulkanDRHI::bindVertexBuffers(DynamicBuffer* vertexBuffer, DynamicCommandBuffer* commandBuffer)
    {
        auto vkVertexBuffer = vertexBuffer->getVulkanBuffer();
        auto vkCommandBuffer = commandBuffer->getVulkanCommandBuffer();
        const VkDeviceSize offsets[1] = { 0 };
        vkCmdBindVertexBuffers(vkCommandBuffer, 0, 1, &vkVertexBuffer, offsets);
    }

    void VulkanDRHI::bindIndexBuffer(DynamicBuffer* indexBuffer, DynamicCommandBuffer* commandBuffer, uint32_t indexType)
    {
        auto vkIndexBuffer = indexBuffer->getVulkanBuffer();
        auto vkCommandBuffer = commandBuffer->getVulkanCommandBuffer();
        vkCmdBindIndexBuffer(vkCommandBuffer, vkIndexBuffer, 0, (VkIndexType)indexType);
    }

    void VulkanDRHI::createDynamicBuffer(DynamicBuffer* buffer, DynamicDeviceMemory* deviceMemory, DynamicCommandPool* commandPool, uint64_t bufferSize, void* bufferData, uint32_t usage, uint32_t memoryProperty)
    {
        auto vkCommandPool = commandPool->getVulkanCommandPool();
        VulkanBuffer::createDynamicBuffer(buffer, deviceMemory, bufferSize, bufferData, &_device, &_physicalDevice, &vkCommandPool, &_graphicQueue, usage, memoryProperty);
    }

    void VulkanDRHI::createUniformBuffer(std::vector<DynamicBuffer>* uniformBuffers, std::vector<DynamicDeviceMemory>* uniformBuffersMemory, std::vector<void*>* uniformBuffersMapped, uint32_t bufferSize)
    {
        uniformBuffers->resize(bufferSize);
        uniformBuffersMemory->resize(bufferSize);
        uniformBuffersMapped->resize(bufferSize);

        for (size_t i = 0; i < bufferSize; i++)
        {
            VkBuffer vkUniformBuffer{};
            VkDeviceMemory vkUniformBufferMemory{};

            VulkanBuffer::createBuffer(&_device, &_physicalDevice, bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, &vkUniformBuffer, &vkUniformBufferMemory);

            vkMapMemory(_device, vkUniformBufferMemory, 0, bufferSize, 0, &(*uniformBuffersMapped)[i]);

            (*uniformBuffers)[i].internalID = vkUniformBuffer;
            (*uniformBuffersMemory)[i].internalID = vkUniformBufferMemory;
        }
    }
    
    void VulkanDRHI::createUniformBuffer(DynamicBuffer* uniformBuffer, DynamicDeviceMemory* uniformBufferMemory, void** uniformBufferMapped, uint32_t bufferSize)
    {
        VkBuffer vkUniformBuffer{};
        VkDeviceMemory vkUniformBufferMemory{};
        //void* bufferMapped{ nullptr };
        VulkanBuffer::createBuffer(&_device, &_physicalDevice, bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, &vkUniformBuffer, &vkUniformBufferMemory);
        vkMapMemory(_device, vkUniformBufferMemory, 0, bufferSize, 0, uniformBufferMapped);
        uniformBuffer->internalID = vkUniformBuffer;
        uniformBufferMemory->internalID = vkUniformBufferMemory;
        //uniformBufferMapped = bufferMapped;
    }

    void VulkanDRHI::clearBuffer(DynamicBuffer* buffer, DynamicDeviceMemory* memory)
    {
        if (!buffer->valid() || !memory->valid()) return;
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
        
        VulkanPipeline::VulkanPipelineCreateInfo pci{};

        VkShaderModule vulkanVertex{}, vulkanFragment{};
        uint32_t shaderCount = 0;

        if (info.vertexShader)
        {
            auto vertex = readFile(info.vertexShader);
            vulkanVertex = createShaderModule(vertex, &_device);
            pci.vertexShader = vulkanVertex;
            shaderCount++;
        }

        if (info.fragmentShader)
        {
            auto fragment = readFile(info.fragmentShader);
            vulkanFragment = createShaderModule(fragment, &_device);
            pci.fragmentShader = vulkanFragment;
            shaderCount++;
        }

        pci.shaderCount = shaderCount;

        pci.dynamicDepthBias = info.dynamicDepthBias;
        pci.colorImageFormat = (VkFormat)info.colorImageFormat;
        pci.depthImageFormat = (VkFormat)info.depthImageFormat;
        pci.cullMode = (VkCullModeFlagBits)info.cullMode;
        pci.includeStencil = info.includeStencil;
        pci.sampleCounts = info.sampleCounts;
        pci.subpass = info.subpass;
        if(info.renderPass) pci.renderPass = info.renderPass->getVulkanRenderPass();

        auto vkVertexInputBinding = info.vertexInputBinding.getVulkanVertexInputBindingDescription();

        std::vector<VkVertexInputAttributeDescription> vkVertexInputAttribute{};

        for (int i = 0; i < info.vertexInputAttributes.size(); ++i)
        {
            vkVertexInputAttribute.emplace_back(info.vertexInputAttributes[i].getVulkanVertexInputAttributeDescription());
        }

        if (pci.renderPass)
        {
            VulkanPipeline::createGraphicsPipeline(&vkpipeline, &vkpipelineLayout, &_pipelineCache, pci, &_device, vkVertexInputBinding, vkVertexInputAttribute);
        }
        else
        {
            // using dynamic rendering feature
            VulkanPipeline::createGraphicsPipelineKHR(&vkpipeline, &vkpipelineLayout, &_pipelineCache, pci, &_device, vkVertexInputBinding, vkVertexInputAttribute);
        }     

        pipeline->internalID = vkpipeline;
    }

    void VulkanDRHI::bindPipeline(DynamicPipeline pipeline, DynamicCommandBuffer* commandBuffer ,uint32_t bindPoint)
    {
        auto vkcommandBuffer = commandBuffer->getVulkanCommandBuffer();
        vkCmdBindPipeline(vkcommandBuffer, (VkPipelineBindPoint)bindPoint, pipeline.getVulkanPipeline());
        commandBuffer->internalID = vkcommandBuffer;
    }

    void VulkanDRHI::clearPipeline(DynamicPipeline* pipeline, DynamicPipelineLayout* pipelineLayout)
    {
        if (!pipeline->valid() || !pipelineLayout->valid()) return;
        vkDestroyPipelineLayout(_device, pipelineLayout->getVulkanPipelineLayout(), nullptr);
        vkDestroyPipeline(_device, pipeline->getVulkanPipeline(), nullptr);
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
       
    }

    void VulkanDRHI::createDescriptorPool(DynamicDescriptorPool* descriptorPool, DynamicDescriptorPoolCreateInfo* ci)
    {
        VkDescriptorPool vkdescriptorPool{};
        VkDescriptorPoolCreateInfo vci{};
        vci.flags = ci->flags;
        vci.maxSets = ci->maxSets;
        

        std::vector<VkDescriptorPoolSize> vkpoolSizes{};

        for (int i = 0; i < ci->pPoolSizes->size(); ++i)
        {
            auto vkpoolsize = VkDescriptorPoolSize();
            vkpoolsize.descriptorCount = (*ci->pPoolSizes)[i].descriptorCount;
            vkpoolsize.type = (VkDescriptorType)(*ci->pPoolSizes)[i].type;
            vkpoolSizes.push_back(vkpoolsize);
        }

        vci.poolSizeCount = static_cast<uint32_t>(vkpoolSizes.size());
        vci.pPoolSizes = vkpoolSizes.data();

        VulkanDescriptor::createDescriptorPool(&vkdescriptorPool, &vci, &_device);

        descriptorPool->internalID = vkdescriptorPool;
    }
    
    void VulkanDRHI::createDescriptorSet(DynamicDescriptorSet* descriptorSet, DynamicDescriptorSetLayout* descriptorSetLayout, DynamicDescriptorPool* descriptorPool, std::vector<DynamicWriteDescriptorSet>* wds, uint32_t imageCount)
    {
        VkDescriptorSet vkdescriptorSet{};
        VkDescriptorPool vkdescriptorPool = descriptorPool->getVulkanDescriptorPool();
        VkDescriptorSetLayout vkdescriptorSetLayout = descriptorSetLayout->getVulkanDescriptorSetLayout();
        VulkanDescriptor::createDescriptorSet(&vkdescriptorSet, &vkdescriptorPool, &vkdescriptorSetLayout, wds, imageCount, &_device);
        descriptorSet->internalID = vkdescriptorSet;
        descriptorSetLayout->internalID = vkdescriptorSetLayout;
    }

    void VulkanDRHI::bindDescriptorSets(DynamicDescriptorSet* descriptorSet, DynamicPipelineLayout pipelineLayout, DynamicCommandBuffer* commandBuffer ,uint32_t bindPoint)
    {
        auto vkDescriptorSet = descriptorSet->getVulkanDescriptorSet();
        auto vkcommandBuffer = commandBuffer->getVulkanCommandBuffer();
        vkCmdBindDescriptorSets(vkcommandBuffer, (VkPipelineBindPoint)bindPoint, pipelineLayout.getVulkanPipelineLayout(), 0, 1, &vkDescriptorSet, 0, nullptr);
        descriptorSet->internalID = vkDescriptorSet;
    }

    void VulkanDRHI::createDescriptorSetLayout(DynamicDescriptorSetLayout* descriptorSetLayout, std::vector<DynamicDescriptorSetLayoutBinding>* dsbs)
    {
        VkDescriptorSetLayout vkdescriptorSetLayout{};

        VulkanDescriptor::createDescriptorSetLayout(&vkdescriptorSetLayout, dsbs, &_device);

        descriptorSetLayout->internalID = vkdescriptorSetLayout;
    }

    void VulkanDRHI::clearDescriptorPool(DynamicDescriptorPool* descriptorPool)
    {
        vkDestroyDescriptorPool(_device, descriptorPool->getVulkanDescriptorPool(), nullptr);
    }

    void VulkanDRHI::clearDescriptorSetLayout(DynamicDescriptorSetLayout* descriptorSetLayout)
    {
        vkDestroyDescriptorSetLayout(_device, descriptorSetLayout->getVulkanDescriptorSetLayout(), nullptr);
    }

    void VulkanDRHI::freeDescriptorSets(DynamicDescriptorSet* descriptorSet, DynamicDescriptorPool* descriptorPool)
    {
        auto vkdesciptorSet = descriptorSet->getVulkanDescriptorSet();
        vkFreeDescriptorSets(_device, descriptorPool->getVulkanDescriptorPool(), 1, &vkdesciptorSet);
    }
    //-----------------------------------------------------------------------------------------






    //------------------------------------texture functions-------------------------------------
    void VulkanDRHI::createTextureImage(DynamicImage* textureImage, DynamicDeviceMemory* textureMemory, DynamicCommandPool* commandPool, int texWidth, int texHeight, int texChannels, stbi_uc* pixels)
    {
        auto vkCommandPool = commandPool->getVulkanCommandPool();
        VkImage vkImage;
        VkDeviceMemory vkMemory;
        VulkanImage::createTextureImage(&vkImage, &vkMemory, texWidth, texHeight, texChannels, pixels, &_device, &_physicalDevice, &_graphicQueue, &vkCommandPool);
        textureImage->internalID = vkImage;
        textureMemory->internalID = vkMemory;
    }

    void VulkanDRHI::createTextureImage(DynamicImage* textureImage, DynamicDeviceMemory* textureMemory, DynamicImageCreateInfo info, DynamicCommandPool* commandPool, stbi_uc* pixels)
    {
        auto vkCommandPool = commandPool->getVulkanCommandPool();
        VkImage vkImage;
        VkDeviceMemory vkMemory;
        VulkanImage::createTextureImage(&vkImage, info, &vkMemory, pixels, &_device, &_physicalDevice, &_graphicQueue, &vkCommandPool);
        textureImage->internalID = vkImage;
        textureMemory->internalID = vkMemory;
    }

    void VulkanDRHI::createTextureSampler(DynamicSampler* textureSampler)
    {
        VkSampler vkSampler;
        VulkanImage::createTextureSampler(&vkSampler, &_physicalDevice, &_device);
        textureSampler->internalID = vkSampler;
    }

    void VulkanDRHI::createCubeTexture(DynamicImage* image, DynamicDeviceMemory* memory, DynamicCommandPool cmdPool, unsigned char* textureData, uint64_t textureSize, uint32_t width, uint32_t height, uint32_t mipLevels, std::vector<size_t> offsets, std::vector<DynamicExtent2D> texSizes)
    {
        VkImage vkimage{};
        VkDeviceMemory vkmemory{};
        VulkanImage::createCubeTextureImage(&vkimage, &vkmemory, cmdPool.getVulkanCommandPool(), textureData, textureSize, width, height, mipLevels, offsets, texSizes, _device, &_physicalDevice, _graphicQueue);
        image->internalID = vkimage;
        memory->internalID = vkmemory;
    }
    //------------------------------------------------------------------------------------------





    //-----------------------------------image functions----------------------------------------
    void VulkanDRHI::createImageView(DynamicImageView* imageView, DynamicImage* image, uint32_t imageFormat, uint32_t imageAspect)
    {
        VkImage vkImage = image->getVulkanImage();
        VkImageView vkTextureImageView = VulkanImage::createImageView(&_device, &vkImage, (VkFormat)imageFormat, (VkImageAspectFlags)imageAspect);
        imageView->internalID = vkTextureImageView;
    }

    void VulkanDRHI::createImageView(DynamicImageView* imageView, DynamicImage* image, DynamicImageViewCreateInfo info)
    {
        VkImage vkImage = image->getVulkanImage();
        VkImageView vkTextureImageView = VulkanImage::createImageView(&_device, &vkImage, info);
        imageView->internalID = vkTextureImageView;
    }

    void VulkanDRHI::createImage(DynamicImage* image, uint32_t width, uint32_t height, 
        uint32_t format, uint32_t imageTiling, uint32_t imageUsageFlagBits, uint32_t memoryPropertyFlags, uint32_t sampleCounts, DynamicDeviceMemory* imageMemory)
    {
        VkImage vkimage{};
        VkDeviceMemory vkmemory{};
        VkFormat vkformat = (VkFormat)format;
        VkImageTiling vkimageTiling = (VkImageTiling)imageTiling;
        VkImageUsageFlagBits vkImageUsageFlagBits = (VkImageUsageFlagBits)imageUsageFlagBits;
        VkMemoryPropertyFlagBits vkMemoryPropertyFlagBits = (VkMemoryPropertyFlagBits)memoryPropertyFlags;
        VkSampleCountFlagBits vkSampleCounts = (VkSampleCountFlagBits)sampleCounts;

        VulkanImage::createImage(&vkimage, width, height, vkformat, vkimageTiling, vkSampleCounts, vkImageUsageFlagBits, vkMemoryPropertyFlagBits, vkmemory, &_device, &_physicalDevice);

        image->internalID = vkimage;
        imageMemory->internalID = vkmemory;
    }

    void VulkanDRHI::createImage(DynamicImage* image, DynamicDeviceMemory* imageMemory, DynamicImageCreateInfo info, uint32_t memoryPropertyFlags)
    {
        VkImage vkimage{};
        VkDeviceMemory vkmemory{};
        VkMemoryPropertyFlagBits vkMemoryPropertyFlagBits = (VkMemoryPropertyFlagBits)memoryPropertyFlags;

        VulkanImage::createImage(&vkimage, info, vkmemory, memoryPropertyFlags, &_device, &_physicalDevice);

        image->internalID = vkimage;
        imageMemory->internalID = vkmemory;
    }

    void VulkanDRHI::copyBufferToImage(DynamicBuffer* buffer, DynamicImage* image, DynamicCommandPool* commandPool, uint32_t width, uint32_t height)
    {
        VkBuffer vkbuffer = buffer->getVulkanBuffer();
        VkImage vkimage = image->getVulkanImage();
        auto vkcommandPool = commandPool->getVulkanCommandPool();
        
        VulkanImage::copyBufferToImage(&vkbuffer, &vkimage, width, height, &_graphicQueue, &vkcommandPool, &_device);
        
        buffer->internalID = vkbuffer;
        image->internalID = vkimage;
    }

    void VulkanDRHI::createSampler(DynamicSampler* sampler, DynamicSamplerCreateInfo createInfo)
    {
        VkSampler vksampler{};
        VulkanImage::createSampler(&vksampler, createInfo, &_physicalDevice, &_device);
        sampler->internalID = vksampler;
    }

    void VulkanDRHI::clearImage(DynamicImageView* imageView, DynamicImage* image, DynamicDeviceMemory* memory)
    {
        vkDestroyImageView(_device, std::get<VkImageView>(imageView->internalID), nullptr);
        vkDestroyImage(_device, std::get<VkImage>(image->internalID), nullptr);
        vkFreeMemory(_device, std::get<VkDeviceMemory>(memory->internalID), nullptr);
    }

    void VulkanDRHI::clearSampler(DynamicSampler* sampler)
    {
        vkDestroySampler(_device, std::get<VkSampler>(sampler->internalID), nullptr);
    }


    void VulkanDRHI::createViewportImage(std::vector<DynamicImage>* viewportImages, std::vector<DynamicDeviceMemory>* viewportImageMemorys, DynamicCommandPool* commandPool)
    {
        auto vkcommandPool = commandPool->getVulkanCommandPool();
        viewportImages->resize(_swapChainImages.size());
        viewportImageMemorys->resize(_swapChainImages.size());

        for (uint32_t i = 0; i < _swapChainImages.size(); i++)
        {
            // Create the linear tiled destination image to copy to and to read the memory from
            VkImageCreateInfo imageCreateCI{};
            imageCreateCI.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
            imageCreateCI.imageType = VK_IMAGE_TYPE_2D;
            // Note that vkCmdBlitImage (if supported) will also do format conversions if the swapchain color format would differ
            imageCreateCI.format = VK_FORMAT_B8G8R8A8_UNORM;
            imageCreateCI.extent.width = _swapChainExtent.width;
            imageCreateCI.extent.height = _swapChainExtent.height;
            imageCreateCI.extent.depth = 1;
            imageCreateCI.arrayLayers = 1;
            imageCreateCI.mipLevels = 1;
            imageCreateCI.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
            imageCreateCI.samples = VK_SAMPLE_COUNT_1_BIT;
            imageCreateCI.tiling = VK_IMAGE_TILING_OPTIMAL;
            imageCreateCI.usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
            // Create the image
            VkImage vkimage{};
            if ((vkCreateImage(_device, &imageCreateCI, nullptr, &vkimage)) != VK_SUCCESS)
            {
                throw std::runtime_error("failed to create image");
            }
            VkMemoryRequirements memReqs{};
            vkGetImageMemoryRequirements(_device, vkimage, &memReqs);

            VkMemoryAllocateInfo memAllloc{};
            memAllloc.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
            memAllloc.allocationSize = memReqs.size;
            memAllloc.memoryTypeIndex = getMemoryType(&_physicalDevice, memReqs.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

            VkDeviceMemory vkmemory{};

            if (vkAllocateMemory(_device, &memAllloc, nullptr, &vkmemory) != VK_SUCCESS)
            {
                throw std::runtime_error("failed to allocate memory");
            }

            if (vkBindImageMemory(_device, vkimage, vkmemory, 0) != VK_SUCCESS)
            {
                throw std::runtime_error("failed to allocate memory");
            }

            VkCommandBuffer copyCmd = VulkanCommand::beginSingleTimeCommands(&vkcommandPool, &_device);

            VulkanCommand::insertImageMemoryBarrier(
                &copyCmd,
                &vkimage,
                VK_ACCESS_TRANSFER_READ_BIT,
                VK_ACCESS_MEMORY_READ_BIT,
                VK_IMAGE_LAYOUT_UNDEFINED,
                VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
                VK_PIPELINE_STAGE_TRANSFER_BIT,
                VK_PIPELINE_STAGE_TRANSFER_BIT,
                VkImageSubresourceRange{ VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 });

            VulkanCommand::endSingleTimeCommands(copyCmd, &_graphicQueue, &vkcommandPool, &_device);

            (*viewportImages)[i].internalID = vkimage;
            (*viewportImageMemorys)[i].internalID = vkmemory;
        }
    }

    void VulkanDRHI::createViewportImageViews(std::vector<DynamicImageView>* viewportImageViews, std::vector<DynamicImage>* viewportImages)
    {
        viewportImageViews->resize(viewportImages->size());

        for (uint32_t i = 0; i < viewportImages->size(); i++)
        {
            VkImage scImages = (*viewportImages)[i].getVulkanImage();
            (*viewportImageViews)[i].internalID = VulkanImage::createImageView(&_device, &scImages, VK_FORMAT_B8G8R8A8_UNORM, VK_IMAGE_ASPECT_COLOR_BIT);
        }
    }

    void VulkanDRHI::transitionImageLayout(DynamicImage* image, DynamicCommandPool* commandPool, uint32_t format, uint32_t oldLayout, uint32_t newLayout)
    {
        auto vkimage = image->getVulkanImage();
        auto vkcommandPool = commandPool->getVulkanCommandPool();
        VulkanImage::transitionImageLayout(vkimage, (VkFormat)format, (VkImageLayout)oldLayout, (VkImageLayout)newLayout, &_graphicQueue, &vkcommandPool, &_device);
        image->internalID = vkimage;
    }

    void VulkanDRHI::createDepthStencil(DynamicImage* depthImage, DynamicImageView* depthImageView, DynamicDeviceMemory* memory, uint32_t format, uint32_t width, uint32_t height, uint32_t sampleCounts)
    {
        DepthStencil depth{};
        depth.image = depthImage->getVulkanImage();
        depth.view = depthImageView->getVulkanImageView();
        depth.memory = memory->getVulkanDeviceMemory();
        VulkanSwapChain::createDepthStencil(&depth, (VkFormat)format, width, height, sampleCounts, &_device, &_physicalDevice);
        depthImage->internalID = depth.image;
        depthImageView->internalID = depth.view;
        memory->internalID = depth.memory;
    }

    void VulkanDRHI::setImageLayout(DynamicCommandBuffer* cmdBuf, DynamicImage* image, uint32_t aspectMask, uint32_t oldImageLayout, uint32_t newImageLayout)
    {
        VkImageSubresourceRange subresourceRange = {};
        subresourceRange.aspectMask = (VkImageAspectFlags)aspectMask;
        subresourceRange.baseMipLevel = 0;
        subresourceRange.levelCount = 1;
        subresourceRange.layerCount = 1;

        VkImage vkimage = image->getVulkanImage();

        VulkanImage::setImageLayout(cmdBuf->getVulkanCommandBuffer(), vkimage, (VkImageLayout)oldImageLayout, (VkImageLayout)newImageLayout, subresourceRange, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT);
    
        image->internalID = vkimage;
    }

    void VulkanDRHI::setImageLayout(DynamicCommandBuffer* cmdBuf, DynamicImage* image, uint32_t oldImageLayout, uint32_t newImageLayout, DynamicImageSubresourceRange range)
    {
        VkImageSubresourceRange subresourceRange = {};
        subresourceRange.aspectMask = (VkImageAspectFlags)range.aspectMask;
        subresourceRange.baseMipLevel = range.baseMipLevel;
        subresourceRange.levelCount = range.levelCount;
        subresourceRange.layerCount = range.layerCount;
        subresourceRange.baseArrayLayer = range.baseArrayLayer;

        VkImage vkimage = image->getVulkanImage();

        VulkanImage::setImageLayout(cmdBuf->getVulkanCommandBuffer(), vkimage, (VkImageLayout)oldImageLayout, (VkImageLayout)newImageLayout, subresourceRange, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT);

        image->internalID = vkimage;
    }
    //-----------------------------------------------------------------------------------------------






    //------------------------------------ cmd functions --------------------------------------------
    void VulkanDRHI::cmdPushConstants(DynamicPipelineLayout* layout, DynamicCommandBuffer* commandBuffer,uint32_t stage, uint32_t offset, uint32_t size, void* value)
    {
        auto vklayout = layout->getVulkanPipelineLayout();
        auto vkcommandBuffer = commandBuffer->getVulkanCommandBuffer();
        vkCmdPushConstants(vkcommandBuffer, vklayout, (VkShaderStageFlags)stage, offset, size, value);
    }

    void VulkanDRHI::cmdSetDepthBias(DynamicCommandBuffer commandBuffer, float depthBias, float depthBiasClamp, float depthBiasSlope)
    {
        vkCmdSetDepthBias(commandBuffer.getVulkanCommandBuffer(), depthBias, depthBiasClamp, depthBiasSlope);
    }

    void VulkanDRHI::cmdSetViewport(DynamicCommandBuffer cmdBuf, uint32_t firstViewport, uint32_t viewportCount, DynamicViewport viewport)
    {
        VkViewport vkvp{};
        vkvp.x = viewport.x;
        vkvp.y = viewport.y;
        vkvp.height = viewport.height;
        vkvp.width = viewport.width;
        vkvp.maxDepth = vkvp.maxDepth;
        vkvp.minDepth = vkvp.minDepth;
     
        vkCmdSetViewport(cmdBuf.getVulkanCommandBuffer(), firstViewport, viewportCount, &vkvp);
    }

    void VulkanDRHI::cmdSetScissor(DynamicCommandBuffer cmdBuf, uint32_t firstScissor, uint32_t scissorCount, DynamicRect2D scissor)
    {
        VkRect2D r2d{};
        r2d.extent.height = scissor.extent.height;
        r2d.extent.width = scissor.extent.width;
        r2d.offset.x = scissor.offset.x;
        r2d.offset.y = scissor.offset.y;
        vkCmdSetScissor(cmdBuf.getVulkanCommandBuffer(), firstScissor, scissorCount, &r2d);
    }

    void VulkanDRHI::cmdDraw(DynamicCommandBuffer cmdBuf, uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance)
    {
        vkCmdDraw(cmdBuf.getVulkanCommandBuffer(), vertexCount, instanceCount, firstVertex, firstInstance);
    }

    void VulkanDRHI::cmdQueueWaitIdle()
    {
        vkQueueWaitIdle(_graphicQueue);
    }

    void VulkanDRHI::cmdCopyImage(DynamicCommandBuffer cmdBuffer, DynamicImage* srcImage, uint32_t srcImageLayout, DynamicImage* dstImage, uint32_t dstImageLayout, uint32_t regionCount, DynamicImageCopy regions)
    {
        VkImage vkdstImage = dstImage->getVulkanImage();

        VkImageCopy vkregion{};
        vkregion.dstOffset.x = regions.dstOffset.x;
        vkregion.dstOffset.y = regions.dstOffset.y;
        vkregion.dstOffset.z = regions.dstOffset.z;
        vkregion.dstSubresource.aspectMask = (VkImageAspectFlagBits)regions.dstSubresource.aspectMask;
        vkregion.dstSubresource.baseArrayLayer = regions.dstSubresource.baseArrayLayer;
        vkregion.dstSubresource.layerCount = regions.dstSubresource.layerCount;
        vkregion.dstSubresource.mipLevel = regions.dstSubresource.mipLevel;
        vkregion.extent.depth = regions.extent.depth;
        vkregion.extent.height = regions.extent.height;
        vkregion.extent.width = regions.extent.width;
        vkregion.srcOffset.x = regions.srcOffset.x;
        vkregion.srcOffset.y = regions.srcOffset.y;
        vkregion.srcOffset.z = regions.srcOffset.z;
        vkregion.srcSubresource.aspectMask = (VkImageAspectFlagBits)regions.srcSubresource.aspectMask;
        vkregion.srcSubresource.baseArrayLayer = regions.srcSubresource.baseArrayLayer;
        vkregion.srcSubresource.layerCount = regions.srcSubresource.layerCount;
        vkregion.srcSubresource.mipLevel = regions.srcSubresource.mipLevel;

        vkCmdCopyImage(cmdBuffer.getVulkanCommandBuffer(), srcImage->getVulkanImage(), (VkImageLayout)srcImageLayout,
            vkdstImage, (VkImageLayout)dstImageLayout, regionCount, &vkregion);

        dstImage->internalID = vkdstImage;
    }
    //-----------------------------------------------------------------------------------------------

    




    //----------------------------------- render pass functions -------------------------------------
    void VulkanDRHI::createRenderPass(DynamicRenderPass* renderPass, DynamicRenderPassCreateInfo* createInfo)
    {
        VulkanRenderPass::createRenderPass(renderPass, createInfo, _device);
    }

    void VulkanDRHI::beginRenderPass(DynamicCommandBuffer* cmdBuffer, DynamicRenderPassBeginInfo* info, uint32_t subpassContents)
    {
        VkClearValue clearVs[1]{};
        clearVs[0].color = { {0.0f,0.0f,0.0f,1.0f} };

        VkExtent2D e2d{};
        e2d.height = info->renderArea.extent.height;
        e2d.width = info->renderArea.extent.width;

        VkOffset2D o2d{};
        o2d.x = info->renderArea.offset.x;
        o2d.y = info->renderArea.offset.y;

        VkRect2D r2d{};
        r2d.extent = e2d;
        r2d.offset = o2d;

        VkRenderPassBeginInfo bi{};
        bi.clearValueCount = 1;
        bi.framebuffer = info->framebuffer.getVulkanFramebuffer();
        bi.pClearValues = clearVs;
        bi.renderArea = r2d;
        bi.renderPass = info->renderPass.getVulkanRenderPass();
        bi.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        vkCmdBeginRenderPass(cmdBuffer->getVulkanCommandBuffer(), &bi, (VkSubpassContents)subpassContents);
    }

    void VulkanDRHI::endRenderPass(DynamicCommandBuffer* cmdBuffer)
    {
        vkCmdEndRenderPass(cmdBuffer->getVulkanCommandBuffer());
    }

    void VulkanDRHI::clearRenderPass(DynamicRenderPass* renderPass)
    {
        vkDestroyRenderPass(_device, renderPass->getVulkanRenderPass(), nullptr);
        renderPass->internalID = nullptr;
    }
    //-----------------------------------------------------------------------------------------------







    //----------------------------------- framebuffer functions -------------------------------------
    void VulkanDRHI::createFramebuffer(DynamicFramebuffer* frameBuffer, DynamicFramebufferCreateInfo* createInfo)
    {
        VulkanFramebuffer::createFramebuffer(frameBuffer, createInfo, _device);
    }

    void VulkanDRHI::clearFramebuffer(DynamicFramebuffer* frameBuffer)
    {
        vkDestroyFramebuffer(_device, frameBuffer->getVulkanFramebuffer(), nullptr);
        frameBuffer->internalID = nullptr;
    }
    //-----------------------------------------------------------------------------------------------





    



    //----------------------------------- ray tracing functions -------------------------------------
    void VulkanDRHI::initRayTracing()
    {
        VkPhysicalDeviceRayTracingPipelinePropertiesKHR  rayTracingPipelineProperties{};
        VkPhysicalDeviceAccelerationStructureFeaturesKHR accelerationStructureFeatures{};
        rayTracingPipelineProperties.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_TRACING_PIPELINE_PROPERTIES_KHR;
        VkPhysicalDeviceProperties2 deviceProperties2{};
        deviceProperties2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2;
        deviceProperties2.pNext = &rayTracingPipelineProperties;
        vkGetPhysicalDeviceProperties2(_physicalDevice, &deviceProperties2);
        accelerationStructureFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ACCELERATION_STRUCTURE_FEATURES_KHR;
        VkPhysicalDeviceFeatures2 deviceFeatures2{};
        deviceFeatures2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2;
        deviceFeatures2.pNext = &accelerationStructureFeatures;
        vkGetPhysicalDeviceFeatures2(_physicalDevice, &deviceFeatures2);
    }

    void VulkanDRHI::createBLAS()
    {

    }

    void VulkanDRHI::createTLAS()
    {

    }
    //-----------------------------------------------------------------------------------------------




    bool VulkanDRHI::getGraphicsQueueState()
    {
        if (vkQueueWaitIdle(_graphicQueue) != VK_SUCCESS) {
            return false;
        }
        else
        {
            return true;
        }
    }
}