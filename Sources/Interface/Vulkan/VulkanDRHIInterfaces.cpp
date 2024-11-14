#include"../../Include/Vulkan/VulkanDRHI.h"
#include"../../Include/Vulkan/VulkanShader.h"
#include"../../Include/CoreFunction.h"

namespace DRHI
{
    //-------------------------------------command functions------------------------------------
    void VulkanDRHI::submitCommandBuffersLists(std::vector<std::vector<DynamicCommandBuffer>> commandBuffersLists)
    {
        for (uint32_t i = 0; i < commandBuffersLists.size(); ++i)
        {
            std::vector<VkCommandBuffer> vkcommandBuffers{};
            for (uint32_t j = 0; i < commandBuffersLists[i].size(); ++j)
            {
                vkcommandBuffers.push_back(commandBuffersLists[i][j].getVulkanCommandBuffer());
            }

            _commandBuffersLists.push_back(vkcommandBuffers);
        }
    }
    
    void VulkanDRHI::createCommandPool(DynamicCommandPool* commandPool)
    {
        VkCommandPool vkcommandPool{};
        VulkanCommand::createCommandPool(&vkcommandPool, &_device, _queueFamilyIndices);
        commandPool->internalID = vkcommandPool;
    }
    
    void VulkanDRHI::createCommandBuffers(std::vector<DynamicCommandBuffer>* commandBuffers, DynamicCommandPool* commandPool)
    {
        std::vector<VkCommandBuffer> vkcommandbuffers{};
        auto vkcommandPool = commandPool->getVulkanCommandPool();
        VulkanCommand::createCommandBuffers(&vkcommandbuffers, &vkcommandPool, VK_COMMAND_BUFFER_LEVEL_PRIMARY, &_device);
        commandPool->internalID = vkcommandPool;

        commandBuffers->resize(vkcommandbuffers.size());
        for (uint32_t i = 0; i < vkcommandbuffers.size(); ++i)
        {
            (*commandBuffers)[i].internalID = vkcommandbuffers[i];
        }
    }
    
    void VulkanDRHI::beginCommandBuffer(uint32_t index)
    {
        VkCommandBufferBeginInfo cmdBufferBeginInfo{};
        cmdBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        //cmdBufferBeginInfo.flags = VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT;

        vkBeginCommandBuffer(_commandBuffers[index], &cmdBufferBeginInfo);
    }

    void VulkanDRHI::beginCommandBuffer(DynamicCommandBuffer commandBuffer)
    {
        VkCommandBuffer vkCommandBuffer = commandBuffer.getVulkanCommandBuffer();

        VkCommandBufferBeginInfo cmdBufferBeginInfo{};
        cmdBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        cmdBufferBeginInfo.flags = VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT;
        vkBeginCommandBuffer(vkCommandBuffer, &cmdBufferBeginInfo);
    }

    void VulkanDRHI::beginRendering(uint32_t index, DynamicCommandBuffer commandBuffer)
    {
        VkCommandBuffer vkCommandBuffer = commandBuffer.getVulkanCommandBuffer();
        VulkanCommand::beginRendering(vkCommandBuffer, _swapChainImages[index], _depthStencil.image, _swapChainImageViews[index], _depthStencil.view, _viewPortWidth, _viewPortHeight, false);
    }

    void VulkanDRHI::beginRendering(uint32_t index)
    {
        VulkanCommand::beginRendering(_commandBuffers[index], _swapChainImages[index], _depthStencil.image, _swapChainImageViews[index], _depthStencil.view, _viewPortWidth, _viewPortHeight, true);
    }

    uint32_t VulkanDRHI::getCommandBufferSize()
    {
        return _commandBuffers.size();
    }

    void VulkanDRHI::endCommandBuffer(uint32_t index)
    {
        vkEndCommandBuffer(_commandBuffers[index]);
    }

    void VulkanDRHI::endCommandBuffer(DynamicCommandBuffer commandBuffer)
    {
        VkCommandBuffer vkCommandBuffer = commandBuffer.getVulkanCommandBuffer();
        vkEndCommandBuffer(vkCommandBuffer);
    }

    void VulkanDRHI::endRendering(DynamicCommandBuffer commandBuffer)
    {
        VkCommandBuffer vkCommandBuffer = commandBuffer.getVulkanCommandBuffer();
        vkCmdEndRendering(vkCommandBuffer);
    }

    void VulkanDRHI::endRendering(uint32_t index)
    {
        vkCmdEndRendering(_commandBuffers[index]);
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

    void VulkanDRHI::beginInsertMemoryBarrier(uint32_t index, DynamicCommandBuffer commandBuffer)
    {
        auto vkcommandBuffer = commandBuffer.getVulkanCommandBuffer();
        VulkanCommand::insertImageMemoryBarrier(vkcommandBuffer, _swapChainImages[index], 0,
            VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
            VK_IMAGE_LAYOUT_UNDEFINED,
            VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
            VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
            VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
            VkImageSubresourceRange{ VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 });

        VulkanCommand::insertImageMemoryBarrier(vkcommandBuffer, _depthStencil.image, 0,
            VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT,
            VK_IMAGE_LAYOUT_UNDEFINED,
            VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
            VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT,
            VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT,
            VkImageSubresourceRange{ VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT, 0, 1, 0, 1 });
    }

    void VulkanDRHI::endInsterMemoryBarrier(uint32_t index, DynamicCommandBuffer commandBuffer)
    {
        auto vkcommandBuffer = commandBuffer.getVulkanCommandBuffer();
        VulkanCommand::insertImageMemoryBarrier(vkcommandBuffer, _swapChainImages[index],
            VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
            0,
            VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
            VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
            VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
            VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,
            VkImageSubresourceRange{ VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 });
    }

    void VulkanDRHI::beginInsertMemoryBarrier(uint32_t index)
    {
        VulkanCommand::insertImageMemoryBarrier(_commandBuffers[index], _swapChainImages[index], 0,
            VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
            VK_IMAGE_LAYOUT_UNDEFINED,
            VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
            VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
            VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
            VkImageSubresourceRange{ VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 });

        VulkanCommand::insertImageMemoryBarrier(_commandBuffers[index], _depthStencil.image, 0,
            VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT,
            VK_IMAGE_LAYOUT_UNDEFINED,
            VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
            VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT,
            VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT,
            VkImageSubresourceRange{ VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT, 0, 1, 0, 1 });
    }

    void VulkanDRHI::endInsterMemoryBarrier(uint32_t index)
    {
        VulkanCommand::insertImageMemoryBarrier(_commandBuffers[index], _swapChainImages[index],
            VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
            0,
            VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
            VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
            VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
            VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,
            VkImageSubresourceRange{ VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 });
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
    void VulkanDRHI::createImageView(DynamicImageView* imageView, DynamicImage* image, uint32_t imageFormat, uint32_t imageAspect)
    {
        VkImage vkImage = image->getVulkanImage();
        VkImageView vkTextureImageView = VulkanImage::createImageView(&_device, &vkImage, (VkFormat)imageFormat, (VkImageAspectFlags)imageAspect);
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


    void VulkanDRHI::createViewportImage(std::vector<DynamicImage>* viewportImages, std::vector<DynamicDeviceMemory>* viewportImageMemorys)
    {
        viewportImages->resize(_swapChainImages.size());
        viewportImageMemorys->resize(_swapChainImages.size());

        for (uint32_t i = 0; i < _swapChainImages.size(); i++)
        {
            // Create the linear tiled destination image to copy to and to read the memory from
            VkImageCreateInfo imageCreateCI{};
            imageCreateCI.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
            imageCreateCI.imageType = VK_IMAGE_TYPE_2D;
            // Note that vkCmdBlitImage (if supported) will also do format conversions if the swapchain color format would differ
            imageCreateCI.format = VK_FORMAT_B8G8R8A8_SRGB;
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

            VkCommandBuffer copyCmd = VulkanCommand::beginSingleTimeCommands(&_commandPool, &_device);

            VulkanCommand::insertImageMemoryBarrier(
                copyCmd,
                vkimage,
                VK_ACCESS_TRANSFER_READ_BIT,
                VK_ACCESS_MEMORY_READ_BIT,
                VK_IMAGE_LAYOUT_UNDEFINED,
                VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
                VK_PIPELINE_STAGE_TRANSFER_BIT,
                VK_PIPELINE_STAGE_TRANSFER_BIT,
                VkImageSubresourceRange{ VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 });

            VulkanCommand::endSingleTimeCommands(copyCmd, &_graphicQueue, &_commandPool, &_device);

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
            (*viewportImageViews)[i].internalID = VulkanImage::createImageView(&_device, &scImages, VK_FORMAT_B8G8R8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT);
        }
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