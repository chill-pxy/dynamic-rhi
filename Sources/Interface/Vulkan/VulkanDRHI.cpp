#include<stdexcept>

#include"../../Include/Vulkan/VulkanDRHI.h"
#include"../../Include/Vulkan/VulkanShader.h"
#include"../../Include/CoreFunction.h"

namespace DRHI
{
    //------------------------------------------------------//
    //------------------------------------------------------//
    //------------------public function---------------------//
    //------------------------------------------------------//
    //------------------------------------------------------//
	VulkanDRHI::VulkanDRHI(RHICreateInfo createInfo)
	{
        _platformInfo = createInfo.platformInfo;

        _viewPortWidth = _platformInfo.width;
        _viewPortHeight = _platformInfo.height;
	}

	void VulkanDRHI::clean()
	{
        cleanSwapChain(&_device, &_swapChainFramebuffers, &_swapChainImageViews, &_swapChain);

        vkDestroyPipeline(_device, _graphicsPipeline, nullptr);
        vkDestroyPipelineLayout(_device, _pipelineLayout, nullptr);

        /*for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
            vkDestroySemaphore(device, renderFinishedSemaphores[i], nullptr);
            vkDestroySemaphore(device, imageAvailableSemaphores[i], nullptr);
            vkDestroyFence(device, inFlightFences[i], nullptr);
        }*/

        vkDestroyCommandPool(_device, _commandPool, nullptr);

        vkDestroySemaphore(_device, _semaphores.presentComplete, nullptr);
        vkDestroySemaphore(_device, _semaphores.renderComplete, nullptr);

        vkDestroyDevice(_device, nullptr);

        //if (enableValidationLayers) {
        //    DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
        //}

        vkDestroySurfaceKHR(_instance, _surface, nullptr);
        vkDestroyInstance(_instance, nullptr);
	}

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

	void VulkanDRHI::initialize()
	{
		createInstance(&_instance);
		createSurface(&_surface, &_instance, _platformInfo);
		
        pickPhysicalDevice(&_physicalDevice, &_instance, 0);
		pickGraphicQueueFamily(&_physicalDevice, (uint32_t)-1);
		createLogicalDevice(&_device, &_physicalDevice, &_graphicQueue, &_presentQueue, &_surface, &_queueFamilyIndices);
		
        createSwapChain(&_swapChain, &_physicalDevice, &_device, &_surface, _platformInfo.window, &_swapChainImages, &_swapChainImageFormat, &_swapChainExtent, &_viewPortWidth, &_viewPortHeight);
		createImageViews(&_device, &_swapChainImageViews, &_swapChainImages, &_swapChainImageFormat);
        createDepthStencil(&_depthStencil, _depthFormat, _viewPortWidth, _viewPortHeight, &_device, &_physicalDevice);

        createCommandPool(&_commandPool, &_device, _queueFamilyIndices);
        createCommandBuffers(&_commandBuffers, &_commandPool, &_device); 

        createPipelineCache(&_pipelineCache, &_device);
        
        VulkanDescriptor::createDescriptorSetLayout(&_descriptorSetLayout, &_device);
        VulkanDescriptor::createDescriptorPool(&_descriptorPool, &_device);
        //createDescriptorSet(&_descriptorSet, &_descriptorPool, &_descriptorSetLayout, 1, &_device);
        //createDescriptorSets(&_descriptorSets, &_descriptorSetLayout, &_descriptorPool, &_device);

        createSemaphore(&_semaphores, &_device);

        createSynchronizationPrimitives(&_waitFences, _commandBuffers.size(), &_device);

        //initialize submit info
        /** @brief Pipeline stages used to wait at for graphics queue submissions */
        _submitInfo = VkSubmitInfo{};
        _submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        _submitInfo.pWaitDstStageMask = &_submitPipelineStages;
        _submitInfo.waitSemaphoreCount = 1;
        _submitInfo.pWaitSemaphores = &_semaphores.presentComplete;
        _submitInfo.signalSemaphoreCount = 1;
        _submitInfo.pSignalSemaphores = &_semaphores.renderComplete;

	}

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

        //vkCmdBindPipeline(_commandBuffers[index], VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.getVulkanPipeline());
    }

    void VulkanDRHI::bindPipeline(DynamicPipeline pipeline, uint32_t bindPoint, uint32_t index)
    {
        vkCmdBindPipeline(_commandBuffers[index], (VkPipelineBindPoint)bindPoint, pipeline.getVulkanPipeline());
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

    void VulkanDRHI::modelDraw(DynamicBuffer* vertexBuffer, DynamicBuffer* indexBuffer, uint32_t indexSize, uint32_t index)
    {
        auto vkVertexBuffer = vertexBuffer->getVulkanBuffer();
        const VkDeviceSize offsets[1] = { 0 };
        vkCmdBindVertexBuffers(_commandBuffers[index], 0, 1, &vkVertexBuffer, offsets);

        auto vkIndexBuffer = indexBuffer->getVulkanBuffer();
        vkCmdBindIndexBuffer(_commandBuffers[index], vkIndexBuffer, 0, VK_INDEX_TYPE_UINT32);

        vkCmdBindDescriptorSets(_commandBuffers[index], VK_PIPELINE_BIND_POINT_GRAPHICS, _pipelineLayout, 0, 1, &_descriptorSet, 0, nullptr);

        vkCmdDrawIndexed(_commandBuffers[index], indexSize, 1, 0, 0, 0);
    }

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

    void VulkanDRHI::bindDescriptorSets(DynamicDescriptorSet* descriptorSet, DynamicPipelineLayout pipelineLayout, uint32_t bindPoint, uint32_t index)
    {
        auto vkDescriptorSet = descriptorSet->getVulkanDescriptorSet();
        vkCmdBindDescriptorSets(_commandBuffers[index], (VkPipelineBindPoint)bindPoint, pipelineLayout.getVulkanPipelineLayout(), 0, 1, &vkDescriptorSet, 0, nullptr);
        descriptorSet->internalID = vkDescriptorSet;
    }

    void VulkanDRHI::frameOnTick()
    {
        prepareFrame();
        _submitInfo.commandBufferCount = 1;
        _submitInfo.pCommandBuffers = &_commandBuffers[_currentBuffer];
        if (vkQueueSubmit(_graphicQueue, 1, &_submitInfo, VK_NULL_HANDLE) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to submit queue");
        }
        submitFrame();
    }

    uint32_t VulkanDRHI::getCommandBufferSize()
    {
        return _commandBuffers.size();
    }

    uint32_t VulkanDRHI::getCurrentBuffer()
    {
        return _currentBuffer;
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

    void VulkanDRHI::createDescriptorSet(std::vector<DynamicDescriptorBufferInfo>* descriptor, DynamicImageView textureImageView, DynamicSampler textureSampler)
    {
        VkImageView vkImageView = textureImageView.getVulkanImageView();
        VkSampler vkSampler = textureSampler.getVulkanSampler();
        VulkanDescriptor::createDescriptorSet(&_descriptorSet, &_descriptorPool, &_descriptorSetLayout, 1, &_device, descriptor, &vkImageView, &vkSampler);
    }

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

    void VulkanDRHI::createPipeline(DynamicPipeline* pipeline, DynamicPipelineLayout* pipelineLayout ,PipelineCreateInfo info)
    {
        VkPipeline vkpipeline;
        VkPipelineLayout vkpipelineLayout;

        auto vertex = readFile(info.vertexShader);
        auto fragment = readFile(info.fragmentShader);

        auto vulkanVertex = createShaderModule(vertex, &_device);
        auto vulkanFragment = createShaderModule(fragment, &_device);

        VulkanPipelineCreateInfo pci{};
        pci.vertexShader = vulkanVertex;
        pci.fragmentShader = vulkanFragment;

        auto vkVertexInputBinding = info.vertexInputBinding.getVulkanVertexInputBindingDescription();

        std::vector<VkVertexInputAttributeDescription> vkVertexInputAttribute;
       // vkVertexInputAttribute.resize(info.vertexInputAttributes.size());
        for (int i = 0; i < info.vertexInputAttributes.size(); ++i)
        {
            vkVertexInputAttribute.emplace_back(info.vertexInputAttributes[i].getVulkanVertexInputAttributeDescription());
        }

        createGraphicsPipeline(&vkpipeline, &vkpipelineLayout, &_pipelineCache, pci, &_device,& _descriptorSetLayout, &_swapChainImageFormat, vkVertexInputBinding, vkVertexInputAttribute);
        
        pipeline->internalID = vkpipeline;
        pipelineLayout->internalID = vkpipelineLayout;
    }

    //------------------------------------------------------//
    //------------------------------------------------------//
    //------------------private function--------------------//
    //------------------------------------------------------//
    //------------------------------------------------------//

    //https://github.com/SaschaWillems/Vulkan/blob/master/examples/dynamicrendering/dynamicrendering.cpp#L81
    void VulkanDRHI::insertImageMemoryBarrier(
        VkCommandBuffer cmdbuffer,
        VkImage image,
        VkAccessFlags srcAccessMask,
        VkAccessFlags dstAccessMask,
        VkImageLayout oldImageLayout,
        VkImageLayout newImageLayout,
        VkPipelineStageFlags srcStageMask,
        VkPipelineStageFlags dstStageMask,
        VkImageSubresourceRange subresourceRange)
    {
        VkImageMemoryBarrier imageMemoryBarrier{};
        imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        imageMemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        imageMemoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        imageMemoryBarrier.srcAccessMask = srcAccessMask;
        imageMemoryBarrier.dstAccessMask = dstAccessMask;
        imageMemoryBarrier.oldLayout = oldImageLayout;
        imageMemoryBarrier.newLayout = newImageLayout;
        imageMemoryBarrier.image = image;
        imageMemoryBarrier.subresourceRange = subresourceRange;

        vkCmdPipelineBarrier(
            cmdbuffer,
            srcStageMask,
            dstStageMask,
            0,
            0, nullptr,
            0, nullptr,
            1, &imageMemoryBarrier);
    }

    void VulkanDRHI::windowResize()
    {
        vkDeviceWaitIdle(_device);

        for (auto imageView : _swapChainImageViews) {
            vkDestroyImageView(_device, imageView, nullptr);
        }

        vkDestroySwapchainKHR(_device, _swapChain, nullptr);

        createSwapChain(&_swapChain, &_physicalDevice, &_device, &_surface, _platformInfo.window, &_swapChainImages, &_swapChainImageFormat, &_swapChainExtent, &_viewPortWidth, &_viewPortHeight);

        vkDestroyImageView(_device, _depthStencil.view, nullptr);
        vkDestroyImage(_device, _depthStencil.image, nullptr);
        vkFreeMemory(_device, _depthStencil.memory, nullptr);

        createDepthStencil(&_depthStencil, _depthFormat, _viewPortWidth, _viewPortHeight, &_device, &_physicalDevice);

        vkFreeCommandBuffers(_device, _commandPool, _commandBuffers.size(), _commandBuffers.data());
        createCommandBuffers(&_commandBuffers, &_commandPool, &_device);

        for (auto& fence : _waitFences) 
        {
            vkDestroyFence(_device, fence, nullptr);
        }

        vkDeviceWaitIdle(_device);
    }

    void VulkanDRHI::prepareFrame()
    {
        auto result = vkAcquireNextImageKHR(_device, _swapChain, UINT64_MAX, _semaphores.presentComplete, (VkFence)nullptr, &_currentBuffer);
        if ((result == VK_ERROR_OUT_OF_DATE_KHR) || (result == VK_SUBOPTIMAL_KHR)) {
            if (result == VK_ERROR_OUT_OF_DATE_KHR) {
                windowResize();
            }
            return;
        }
        else {
            if (result != VK_SUCCESS)
            {
                throw std::runtime_error("failed to acquire next image");
            }
        }
    }

    void VulkanDRHI::submitFrame()
    {
        auto result = queuePresent(&_graphicQueue, &_swapChain, &_currentBuffer, &_semaphores.renderComplete);
        // Recreate the swapchain if it's no longer compatible with the surface (OUT_OF_DATE) or no longer optimal for presentation (SUBOPTIMAL)
        if ((result == VK_ERROR_OUT_OF_DATE_KHR) || (result == VK_SUBOPTIMAL_KHR)) {
            windowResize();
            if (result == VK_ERROR_OUT_OF_DATE_KHR) {
                return;
            }
        }
        else {
            if (result != VK_SUCCESS)
            {
                throw std::runtime_error("failed to submit frame");
            }
        }

        if (vkQueueWaitIdle(_graphicQueue) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to wait queue");
        }
    }
}