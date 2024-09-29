#include"../../Include/Vulkan/VulkanDRHI.h"
#include"../../Include/Vulkan/VulkanShader.h"
#include"../../Include/CoreFunction.h"

namespace DRHI
{
    //--------------------------------------clear functions-------------------------------------
    void VulkanDRHI::clearBuffer(DynamicBuffer* buffer, DynamicDeviceMemory* memory)
    {
        vkDestroyBuffer(_device, std::get<VkBuffer>(buffer->internalID), nullptr);
        vkFreeMemory(_device, std::get<VkDeviceMemory>(memory->internalID), nullptr);
    }

    void VulkanDRHI::clearImage(DynamicSampler* sampler, DynamicImageView* imageView, DynamicImage* image, DynamicDeviceMemory* memory)
    {
        vkDestroySampler(_device, std::get<VkSampler>(sampler->internalID), nullptr);
        vkDestroyImageView(_device, std::get<VkImageView>(imageView->internalID), nullptr);

        vkDestroyImage(_device, std::get<VkImage>(image->internalID), nullptr);
        vkFreeMemory(_device, std::get<VkDeviceMemory>(memory->internalID), nullptr);
    }
    //------------------------------------------------------------------------------------------





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

    void VulkanDRHI::bindIndexBuffer(DynamicBuffer* indexBuffer, uint32_t index)
    {
        auto vkIndexBuffer = indexBuffer->getVulkanBuffer();
        vkCmdBindIndexBuffer(_commandBuffers[index], vkIndexBuffer, 0, VK_INDEX_TYPE_UINT32);
    }

    void VulkanDRHI::createDynamicBuffer(DynamicBuffer* buffer, DynamicDeviceMemory* deviceMemory, uint64_t bufferSize, void* bufferData, const char* type)
    {
        VulkanBuffer::createDynamicBuffer(buffer, deviceMemory, bufferSize, bufferData, &_device, &_physicalDevice, &_commandPool, &_graphicQueue, type);
    }

    void VulkanDRHI::createUniformBuffer(std::vector<DynamicBuffer>* uniformBuffers, std::vector<DynamicDeviceMemory>* uniformBuffersMemory, std::vector<void*>* uniformBuffersMapped, uint32_t bufferSize)
    {
        uniformBuffers->resize(MAX_FRAMES_IN_FLIGHT);
        uniformBuffersMemory->resize(MAX_FRAMES_IN_FLIGHT);
        uniformBuffersMapped->resize(MAX_FRAMES_IN_FLIGHT);

        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
        {
            VkBuffer vkUniformBuffer;
            VkDeviceMemory vkUniformBufferMemory;
            VkDescriptorBufferInfo vkdescriptorInfo{};

            VulkanBuffer::createBuffer(&_device, &_physicalDevice, bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, &vkUniformBuffer, &vkUniformBufferMemory);

            vkMapMemory(_device, vkUniformBufferMemory, 0, bufferSize, 0, &(*uniformBuffersMapped)[i]);

            (*uniformBuffers)[i].internalID = vkUniformBuffer;
            (*uniformBuffersMemory)[i].internalID = vkUniformBufferMemory;
        }
    }

    uint32_t VulkanDRHI::getCurrentBuffer()
    {
        return _currentBuffer;
    }
    //----------------------------------------------------------------------------------------






    //------------------------------------pipeline functions----------------------------------
    void VulkanDRHI::createPipeline(DynamicPipeline* pipeline, DynamicPipelineLayout* pipelineLayout, DynamicDescriptorSetLayout* descriptorSetLayout, PipelineCreateInfo info)
    {
        VkPipeline vkpipeline;
        VkPipelineLayout vkpipelineLayout;
        VkDescriptorSetLayout vkdesscriptorSetLayout = descriptorSetLayout->getVulkanDescriptorSetLayout();

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

        VulkanPipeline::createGraphicsPipeline(&vkpipeline, &vkpipelineLayout, &_pipelineCache, pci, &_device, &vkdesscriptorSetLayout, &_swapChainImageFormat, vkVertexInputBinding, vkVertexInputAttribute);

        pipeline->internalID = vkpipeline;
        pipelineLayout->internalID = vkpipelineLayout;
        descriptorSetLayout->internalID = vkdesscriptorSetLayout;
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






    //-------------------------------------descriptor functions-------------------------------
    void VulkanDRHI::createDescriptorSet(DynamicDescriptorSet* descriptorSet, DynamicDescriptorSetLayout* descriptorSetLayout, std::vector<DynamicDescriptorBufferInfo>* descriptor, DynamicImageView textureImageView, DynamicSampler textureSampler)
    {
        VkImageView vkImageView = textureImageView.getVulkanImageView();
        VkSampler vkSampler = textureSampler.getVulkanSampler();
        VkDescriptorSet vkdescriptorSet{};
        VkDescriptorSetLayout vkdescriptorSetLayout = descriptorSetLayout->getVulkanDescriptorSetLayout();
        VulkanDescriptor::createDescriptorSet(&vkdescriptorSet, &_descriptorPool, &vkdescriptorSetLayout, 1, &_device, descriptor, &vkImageView, &vkSampler);
        descriptorSet->internalID = vkdescriptorSet;
        descriptorSetLayout->internalID = vkdescriptorSetLayout;
    }

    void VulkanDRHI::bindDescriptorSets(DynamicDescriptorSet* descriptorSet, DynamicPipelineLayout pipelineLayout, uint32_t bindPoint, uint32_t index)
    {
        auto vkDescriptorSet = descriptorSet->getVulkanDescriptorSet();
        vkCmdBindDescriptorSets(_commandBuffers[index], (VkPipelineBindPoint)bindPoint, pipelineLayout.getVulkanPipelineLayout(), 0, 1, &vkDescriptorSet, 0, nullptr);
        descriptorSet->internalID = vkDescriptorSet;
    }

    void VulkanDRHI::createDescriptorSetLayout(DynamicDescriptorSetLayout* descriptorSetLayout)
    {
        VkDescriptorSetLayout vkdescriptorSetLayout{};

        VulkanDescriptor::createDescriptorSetLayout(&vkdescriptorSetLayout, &_device);

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

    void VulkanDRHI::createImageView(DynamicImageView* imageView, DynamicImage* image)
    {
        VkImage vkImage = image->getVulkanImage();
        VkImageView vkTextureImageView = VulkanImage::createImageView(&_device, &vkImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT);
        imageView->internalID = vkTextureImageView;
    }

    void VulkanDRHI::createTextureSampler(DynamicSampler* textureSampler)
    {
        VkSampler vkSampler;
        VulkanImage::createTextureSampler(&vkSampler, &_physicalDevice, &_device);
        textureSampler->internalID = vkSampler;
    }
    //------------------------------------------------------------------------------------------
}