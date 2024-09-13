#include<stdexcept>

#include<stb_image.h>

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
	}

	void VulkanDRHI::clean()
	{
        cleanSwapChain(&_device, &_swapChainFramebuffers, &_swapChainImageViews, &_swapChain);

        vkDestroyPipeline(_device, _graphicsPipeline, nullptr);
        vkDestroyPipelineLayout(_device, _pipelineLayout, nullptr);

        //for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        //    vkDestroyBuffer(device, uniformBuffers[i], nullptr);
        //    vkFreeMemory(device, uniformBuffersMemory[i], nullptr);
        //}

        vkDestroyDescriptorPool(_device, _descriptorPool, nullptr);

        //vkDestroySampler(device, textureSampler, nullptr);
        //vkDestroyImageView(device, textureImageView, nullptr);

        //vkDestroyImage(device, textureImage, nullptr);
        //vkFreeMemory(device, textureImageMemory, nullptr);

        vkDestroyDescriptorSetLayout(_device, _descriptorSetLayout, nullptr);

        //vkDestroyBuffer(device, indexBuffer, nullptr);
        //vkFreeMemory(device, indexBufferMemory, nullptr);

        //vkDestroyBuffer(device, vertexBuffer, nullptr);
        //vkFreeMemory(device, vertexBufferMemory, nullptr);

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

	void VulkanDRHI::initialize()
	{
		createInstance(&_instance);
		createSurface(&_surface, &_instance, _platformInfo);

        //calculate viewport size
        RECT rect;
        GetWindowRect(_platformInfo.window, &rect);
        _viewPortWidth = rect.right - rect.left;
        _viewPortHeight = rect.bottom - rect.top;
		
        pickPhysicalDevice(&_physicalDevice, &_instance, 0);
		pickGraphicQueueFamily(&_physicalDevice, (uint32_t)-1);
		createLogicalDevice(&_device, &_physicalDevice, &_graphicQueue, &_presentQueue, &_surface, &_queueFamilyIndices);
		
        createSwapChain(&_swapChain, &_physicalDevice, &_device, &_surface, _platformInfo.window, &_swapChainImages, &_swapChainImageFormat, &_swapChainExtent);
		createImageViews(&_device, &_swapChainImageViews, &_swapChainImages, &_swapChainImageFormat);
        createDepthStencil(&_depthStencil, _depthFormat, _viewPortWidth, _viewPortHeight, &_device, &_physicalDevice);

        createCommandPool(&_commandPool, &_device, _queueFamilyIndices);
        createCommandBuffers(&_commandBuffers, &_commandPool, &_device); 
        
        VulkanDescriptor::createDescriptorSetLayout(&_descriptorSetLayout, &_device);
        VulkanDescriptor::createDescriptorPool(&_descriptorPool, &_device);
        //createDescriptorSet(&_descriptorSet, &_descriptorPool, &_descriptorSetLayout, 1, &_device);
        //createDescriptorSets(&_descriptorSets, &_descriptorSetLayout, &_descriptorPool, &_device);

        createSemaphore(&_semaphores, &_device);

        createSynchronizationPrimitives(&_waitFences, _commandBuffers.size(), &_device);

        //initialize submit info
        /** @brief Pipeline stages used to wait at for graphics queue submissions */
        VkPipelineStageFlags submitPipelineStages = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

        _submitInfo = VkSubmitInfo{};
        _submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        _submitInfo.pWaitDstStageMask = &submitPipelineStages;
        _submitInfo.waitSemaphoreCount = 1;
        _submitInfo.pWaitSemaphores = &_semaphores.presentComplete;
        _submitInfo.signalSemaphoreCount = 1;
        _submitInfo.pSignalSemaphores = &_semaphores.renderComplete;

	}

    void VulkanDRHI::prepareCommandBuffer(DynamicBuffer* vertexBuffer, DynamicBuffer* indexBuffer, uint32_t indicesSize)
    {
        VkCommandBufferBeginInfo cmdBufferBeginInfo{};
        cmdBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        for (int32_t i = 0; i < _commandBuffers.size(); ++i)
        {
            vkBeginCommandBuffer(_commandBuffers[i], &cmdBufferBeginInfo);

            // With dynamic rendering there are no subpass dependencies, so we need to take care of proper layout transitions by using barriers
            // This set of barriers prepares the color and depth images for output
            insertImageMemoryBarrier(_commandBuffers[i], _swapChainImages[i], 0,
                VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
                VK_IMAGE_LAYOUT_UNDEFINED,
                VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
                VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
                VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
                VkImageSubresourceRange{ VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 });

            //need to setup depth image
            insertImageMemoryBarrier(_commandBuffers[i], _depthStencil.image, 0,
                VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
                VK_IMAGE_LAYOUT_UNDEFINED,
                VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
                VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
                VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
                VkImageSubresourceRange{ VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 });
            
            // New structures are used to define the attachments used in dynamic rendering
            VkRenderingAttachmentInfoKHR colorAttachment{};
            colorAttachment.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO_KHR;
            colorAttachment.imageView = _swapChainImageViews[i];
            colorAttachment.imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
            colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
            colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
            colorAttachment.clearValue.color = { 0.0f,0.0f,0.0f,0.0f };

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
            renderingInfo.renderArea = { 0, 0,1920, 1080 };
            renderingInfo.layerCount = 1;
            renderingInfo.colorAttachmentCount = 1;
            renderingInfo.pColorAttachments = &colorAttachment;
            renderingInfo.pDepthAttachment = &depthStencilAttachment;
            renderingInfo.pStencilAttachment = &depthStencilAttachment;

            //Begin dynamic rendering
            vkCmdBeginRenderingKHR(_commandBuffers[i], &renderingInfo);

            VkViewport viewport{};
            viewport.width = _viewPortWidth;
            viewport.height = _viewPortHeight;
            viewport.minDepth = 0.0f;
            viewport.maxDepth = 1.0f;

            vkCmdSetViewport(_commandBuffers[i], 0, 1, &viewport);

            VkRect2D scissor{};
            scissor.extent.width = _viewPortWidth;
            scissor.extent.height = _viewPortHeight;
            scissor.offset.x = 0;
            scissor.offset.y = 0;

            vkCmdSetScissor(_commandBuffers[i], 0, 1, &scissor);

            vkCmdBindDescriptorSets(_commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, _pipelineLayout, 0, 1, &_descriptorSet, 0, nullptr);
            vkCmdBindPipeline(_commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, _graphicsPipeline);

            //....
            //....
            // ui draw
            // ....
            // ....
            
            //binding models vertex and index
            auto vkVertexBuffer = vertexBuffer->getVulkanBuffer();
            const VkDeviceSize offsets[1] = { 0 };
            vkCmdBindVertexBuffers(_commandBuffers[i], 0, 1, &vkVertexBuffer, offsets);

            auto vkIndexBuffer = indexBuffer->getVulkanBuffer();
            vkCmdBindIndexBuffer(_commandBuffers[i], vkIndexBuffer, 0, VK_INDEX_TYPE_UINT32);

            vkCmdBindDescriptorSets(_commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, _pipelineLayout, 0, 1, &_descriptorSets[_currentBuffer], 0, nullptr);

            vkCmdDrawIndexed(_commandBuffers[i], indicesSize, 1, 0, 0, 0);
            
            //End dynamic rendering
            vkCmdEndRenderingKHR(_commandBuffers[i]);

            insertImageMemoryBarrier(_commandBuffers[i], _swapChainImages[i], 
                VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
                0,
                VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
                VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
                VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
                VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,
                VkImageSubresourceRange{ VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 });

            vkEndCommandBuffer(_commandBuffers[i]);
        }
    }

    void VulkanDRHI::frameOnTick()
    {
       // vkDeviceWaitIdle(_device);
        prepareFrame();
        _submitInfo.commandBufferCount = 1;
        _submitInfo.pCommandBuffers = &_commandBuffers[_currentBuffer];
        if (vkQueueSubmit(_graphicQueue, 1, &_submitInfo, VK_NULL_HANDLE) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to submit queue");
        }
        submitFrame();
    }

    void VulkanDRHI::createDynamicBuffer(DynamicBuffer* buffer, DynamicDeviceMemory* deviceMemory, uint64_t bufferSize, void* bufferData)
    {
        VulkanBuffer::createDynamicBuffer(buffer, deviceMemory, bufferSize, bufferData, &_device, &_physicalDevice, &_commandPool, &_graphicQueue);
    }

    void VulkanDRHI::createUniformBuffer(std::vector<DynamicBuffer>* uniformBuffers, std::vector<DynamicDeviceMemory>* uniformBuffersMemory, std::vector<void*>* uniformBuffersMapped, uint32_t bufferSize)
    {
        uniformBuffers->resize(MAX_FRAMES_IN_FLIGHT);
        uniformBuffersMemory->resize(MAX_FRAMES_IN_FLIGHT);
        uniformBuffersMapped->resize(MAX_FRAMES_IN_FLIGHT);

        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) 
        {
            DynamicBuffer uniformBuffer = (*uniformBuffers)[i];
            VkBuffer vkUniformBuffer = uniformBuffer.getVulkanBuffer();

            DynamicDeviceMemory uniformBufferDeviceMemory = (*uniformBuffersMemory)[i];
            VkDeviceMemory vkUniformBufferMemory = uniformBufferDeviceMemory.getVulkanDeviceMemory();

            VulkanBuffer::createBuffer(&_device, &_physicalDevice, bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, vkUniformBuffer, vkUniformBufferMemory);

            vkMapMemory(_device, vkUniformBufferMemory, 0, bufferSize, 0, &(*uniformBuffersMapped)[i]);
        }
    }

    void VulkanDRHI::createDescriptorSets(std::vector<DynamicBuffer>* uniformBuffers, uint32_t uniformBufferSize)
    {
        VulkanDescriptor::createDescriptorSets(&_descriptorSets, &_descriptorSetLayout, &_descriptorPool, &_device, uniformBuffers, uniformBufferSize);
    }

    void VulkanDRHI::createTextureImage(DynamicImage* textureImage, int texWidth, int texHeight, stbi_uc* pixels)
    {
        VkImage vkImage;
        VulkanImage::createTextureImage(&vkImage, texWidth, texHeight, pixels, &_device, &_physicalDevice, &_graphicQueue, &_commandPool);
        textureImage->internalID = vkImage;
    }

    void VulkanDRHI::createPipeline(PipelineCreateInfo info)
    {
        auto vertex = readFile(info.vertexShader);
        auto fragment = readFile(info.fragmentShader);

        auto vulkanVertex = createShaderModule(vertex, &_device);
        auto vulkanFragment = createShaderModule(fragment, &_device);

        VulkanPipelineCreateInfo pci{};
        pci.vertexShader = vulkanVertex;
        pci.fragmentShader = vulkanFragment;

        createGraphicsPipeline(&_graphicsPipeline, &_pipelineLayout, pci, &_device,& _descriptorSetLayout, &_swapChainImageFormat);
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

    void VulkanDRHI::prepareFrame()
    {
        auto result = vkAcquireNextImageKHR(_device, _swapChain, UINT64_MAX, _semaphores.presentComplete, (VkFence)nullptr, &_currentBuffer);
        if ((result == VK_ERROR_OUT_OF_DATE_KHR) || (result == VK_SUBOPTIMAL_KHR)) {
            if (result == VK_ERROR_OUT_OF_DATE_KHR) {
                //windowResize();
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
            //windowResize();
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