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

        vkDestroyImage(_device, _depthStencil.image, nullptr);
        vkDestroyImageView(_device, _depthStencil.view, nullptr);
        vkFreeMemory(_device, _depthStencil.memory, nullptr);

        vkDestroyPipelineCache(_device, _pipelineCache, nullptr);

        vkDestroySemaphore(_device, _semaphores.presentComplete, nullptr);
        vkDestroySemaphore(_device, _semaphores.renderComplete, nullptr);

        for (auto f : _waitFences)
        {
            vkDestroyFence(_device, f, nullptr);
        }

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

        VulkanPipeline::createPipelineCache(&_pipelineCache, &_device);

        createSemaphore(&_semaphores, &_device);

        createSynchronizationPrimitives(&_waitFences, MAX_FRAMES_IN_FLIGHT, &_device);

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

    void VulkanDRHI::frameOnTick(std::vector<std::function<void()>> recreatefuncs, std::vector<DynamicCommandBuffer>* commandBuffers)
    {
        if (_swapChain == VK_NULL_HANDLE) return;

        prepareFrame(recreatefuncs);

        std::vector<VkCommandBuffer> vkcommandBuffers{};
        vkcommandBuffers.resize(commandBuffers->size());

        for (uint32_t i = 0; i < vkcommandBuffers.size(); ++i)
        {
            vkcommandBuffers[i] = (*commandBuffers)[i].getVulkanCommandBuffer();
        }

        _submitInfo.commandBufferCount = vkcommandBuffers.size();
        _submitInfo.pCommandBuffers = vkcommandBuffers.data();
        if (vkQueueSubmit(_graphicQueue, 1, &_submitInfo, _waitFences[_currentFrame]) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to submit queue");
        }

        submitFrame(recreatefuncs);
    }

    void VulkanDRHI::drawIndexed(DynamicCommandBuffer* commandBuffer, uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, uint32_t vertexOffset, uint32_t firstInstance)
    {
        auto vkcommandBuffer = commandBuffer->getVulkanCommandBuffer();
        vkCmdDrawIndexed(vkcommandBuffer, indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
        commandBuffer->internalID = vkcommandBuffer;
    }

    void VulkanDRHI::setScissor(DynamicCommandBuffer* commandBuffer, uint32_t firstScissor, uint32_t scissorCount, DynamicRect2D rect)
    {
        auto vkcommandBuffer = commandBuffer->getVulkanCommandBuffer();

        VkRect2D vkrect{};
        VkExtent2D vkextent{};
        VkOffset2D vkoffset{};
        
        vkextent.width = rect.extent.width;
        vkextent.height = rect.extent.height;

        vkoffset.x = rect.offset.x;
        vkoffset.y = rect.offset.y;

        vkrect.extent = vkextent;
        vkrect.offset = vkoffset;

        vkCmdSetScissor(vkcommandBuffer, firstScissor, scissorCount, &vkrect);

        commandBuffer->internalID = vkcommandBuffer;
    }

    uint32_t VulkanDRHI::getCurrentFrame()
    {
        return _currentFrame;
    }




    //------------------------------------------------------//
    //------------------------------------------------------//
    //------------------private function--------------------//
    //------------------------------------------------------//
    //------------------------------------------------------//

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

        vkDestroyImageView(_device, _depthStencil.view, nullptr);
        vkDestroyImage(_device, _depthStencil.image, nullptr);
        vkFreeMemory(_device, _depthStencil.memory, nullptr);

        createDepthStencil(&_depthStencil, _depthFormat, _viewPortWidth, _viewPortHeight, &_device, &_physicalDevice);

        for (auto f : recreatefuncs)
        {
            f();
        }

        for (auto& fence : _waitFences)
        {
            vkDestroyFence(_device, fence, nullptr);
        }

        createSynchronizationPrimitives(&_waitFences, MAX_FRAMES_IN_FLIGHT, &_device);

        vkDeviceWaitIdle(_device);

        _prepare = true;
    }

    void VulkanDRHI::prepareFrame(std::vector<std::function<void()>> recreatefuncs)
    {
        vkWaitForFences(_device, 1, &_waitFences[_currentFrame], VK_TRUE, UINT64_MAX);
        auto result = vkAcquireNextImageKHR(_device, _swapChain, UINT64_MAX, _semaphores.presentComplete, nullptr, &_currentFrame);
        if ((result == VK_ERROR_OUT_OF_DATE_KHR) || (result == VK_SUBOPTIMAL_KHR)) 
        {
            if (result == VK_ERROR_OUT_OF_DATE_KHR) 
            {
                recreate(recreatefuncs);
            }
            return;
        }
        else 
        {
            if (result != VK_SUCCESS)
            {
                throw std::runtime_error("failed to acquire next image");
            }
        }
        vkResetFences(_device, 1, &_waitFences[_currentFrame]);
    }

    void VulkanDRHI::submitFrame(std::vector<std::function<void()>> recreatefuncs)
    {
        auto result = queuePresent(&_graphicQueue, &_swapChain, &_currentFrame, &_semaphores.renderComplete);
        // Recreate the swapchain if it's no longer compatible with the surface (OUT_OF_DATE) or no longer optimal for presentation (SUBOPTIMAL)
        if ((result == VK_ERROR_OUT_OF_DATE_KHR) || (result == VK_SUBOPTIMAL_KHR)) 
        {
            recreate(recreatefuncs);
            if (result == VK_ERROR_OUT_OF_DATE_KHR) 
            {
                return;
            }
        }
        else 
        {
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