#include<stdexcept>

#include"../../Include/Vulkan/VulkanDRHI.h"

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

        //vkDestroyPipeline(_device, _graphicsPipeline, nullptr);
        //vkDestroyPipelineLayout(_device, _pipelineLayout, nullptr);

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
		
        pickPhysicalDevice(&_physicalDevice, &_instance, 0);
		pickGraphicQueueFamily(&_physicalDevice, (uint32_t)-1);
		createLogicalDevice(&_device, &_physicalDevice, &_graphicQueue, &_presentQueue, &_surface, &_queueFamilyIndices);
		
        createSwapChain(&_swapChain, &_physicalDevice, &_device, &_surface, _platformInfo.window, &_swapChainImages, &_swapChainImageFormat, &_swapChainExtent, &_viewPortWidth, &_viewPortHeight);
		createImageViews(&_device, &_swapChainImageViews, &_swapChainImages, &_swapChainImageFormat);
        createDepthStencil(&_depthStencil, _depthFormat, _viewPortWidth, _viewPortHeight, &_device, &_physicalDevice);

        VulkanCommand::createCommandPool(&_commandPool, &_device, _queueFamilyIndices);
        VulkanCommand::createCommandBuffers(&_commandBuffers, &_commandPool, &_device); 

        VulkanPipeline::createPipelineCache(&_pipelineCache, &_device);

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

        _prepare = true;

	}

    void VulkanDRHI::frameOnTick(std::vector<std::function<void()>> recreatefuncs, std::vector<DynamicCommandBuffer> commandBuffers)
    {
        prepareFrame(recreatefuncs);

        std::vector<VkCommandBuffer> vkcommandBuffers{};
        vkcommandBuffers.resize(commandBuffers.size() + 1);

        vkcommandBuffers[0] = (_commandBuffers[_currentBuffer]);
        //vkcommandBuffers[0] = (commandBuffers[0].getVulkanCommandBuffer());

        for (uint32_t i = 1; i < vkcommandBuffers.size(); ++i)
        {
            vkcommandBuffers[i] = (commandBuffers[i - 1].getVulkanCommandBuffer());
        }

        _submitInfo.commandBufferCount = vkcommandBuffers.size();
        _submitInfo.pCommandBuffers = vkcommandBuffers.data();
        if (vkQueueSubmit(_graphicQueue, 1, &_submitInfo, _waitFences[_currentBuffer]) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to submit queue");
        }
        submitFrame(recreatefuncs);
    }

    void VulkanDRHI::frameOnTick(std::vector<std::function<void()>> recreatefuncs)
    {
        prepareFrame(recreatefuncs);

        _submitInfo.commandBufferCount = 1;
        _submitInfo.pCommandBuffers = &_commandBuffers[_currentBuffer];
        if (vkQueueSubmit(_graphicQueue, 1, &_submitInfo, _waitFences[_currentBuffer]) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to submit queue");
        }
        submitFrame(recreatefuncs);
    }

    void VulkanDRHI::drawIndexed(uint32_t index, uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, uint32_t vertexOffset, uint32_t firstInstance)
    {
        vkCmdDrawIndexed(_commandBuffers[index], indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
    }

    void VulkanDRHI::setScissor(uint32_t index, uint32_t firstScissor, uint32_t scissorCount,DynamicRect2D rect)
    {
        VkRect2D vkrect{};
        VkExtent2D vkextent{};
        VkOffset2D vkoffset{};
        
        vkextent.width = rect.extent.width;
        vkextent.height = rect.extent.height;

        vkoffset.x = rect.offset.x;
        vkoffset.y = rect.offset.y;

        vkrect.extent = vkextent;
        vkrect.offset = vkoffset;

        vkCmdSetScissor(_commandBuffers[index], firstScissor, scissorCount, &vkrect);
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

    void VulkanDRHI::recreate(std::vector<std::function<void()>> recreatefuncs)
    {
        if (!_prepare) return;

        _prepare = false;

        vkDeviceWaitIdle(_device);

        for (auto imageView : _swapChainImageViews) {
            vkDestroyImageView(_device, imageView, nullptr);
        }

        createSwapChain(&_swapChain, &_physicalDevice, &_device, &_surface, _platformInfo.window, &_swapChainImages, &_swapChainImageFormat, &_swapChainExtent, &_viewPortWidth, &_viewPortHeight);
        createImageViews(&_device, &_swapChainImageViews, &_swapChainImages, &_swapChainImageFormat);
        //vkDestroySwapchainKHR(_device, _swapChain, nullptr);

        vkDestroyImageView(_device, _depthStencil.view, nullptr);
        vkDestroyImage(_device, _depthStencil.image, nullptr);
        vkFreeMemory(_device, _depthStencil.memory, nullptr);

        createDepthStencil(&_depthStencil, _depthFormat, _viewPortWidth, _viewPortHeight, &_device, &_physicalDevice);

        vkFreeCommandBuffers(_device, _commandPool, _commandBuffers.size(), _commandBuffers.data());
        VulkanCommand::createCommandBuffers(&_commandBuffers, &_commandPool, &_device);

        for (auto f : recreatefuncs)
        {
            f();
        }

        for (auto& fence : _waitFences)
        {
            vkDestroyFence(_device, fence, nullptr);
        }

        createSynchronizationPrimitives(&_waitFences, _commandBuffers.size(), &_device);

        vkDeviceWaitIdle(_device);

        _prepare = true;
    }

    void VulkanDRHI::prepareFrame(std::vector<std::function<void()>> recreatefuncs)
    {
        vkWaitForFences(_device, 1, &_waitFences[_currentBuffer], VK_TRUE, UINT64_MAX);
        auto result = vkAcquireNextImageKHR(_device, _swapChain, UINT64_MAX, _semaphores.presentComplete, (VkFence)nullptr, &_currentBuffer);
        if ((result == VK_ERROR_OUT_OF_DATE_KHR) || (result == VK_SUBOPTIMAL_KHR)) {
            if (result == VK_ERROR_OUT_OF_DATE_KHR) {
                recreate(recreatefuncs);
            }
            return;
        }
        else {
            if (result != VK_SUCCESS)
            {
                throw std::runtime_error("failed to acquire next image");
            }
        }
        vkResetFences(_device, 1, &_waitFences[_currentBuffer]);
    }

    void VulkanDRHI::submitFrame(std::vector<std::function<void()>> recreatefuncs)
    {
        auto result = queuePresent(&_presentQueue, &_swapChain, &_currentBuffer, &_semaphores.renderComplete);
        // Recreate the swapchain if it's no longer compatible with the surface (OUT_OF_DATE) or no longer optimal for presentation (SUBOPTIMAL)
        if ((result == VK_ERROR_OUT_OF_DATE_KHR) || (result == VK_SUBOPTIMAL_KHR)) {
            recreate(recreatefuncs);
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

        if (vkQueueWaitIdle(_presentQueue) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to wait queue");
        }
    }
}