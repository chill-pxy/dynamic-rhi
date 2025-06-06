#include<stdexcept>

#include"../../Include/Vulkan/VulkanDRHI.h"

namespace drhi
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

        _virtualDevice = std::make_unique<DynamicDevice>();
        _virtualPhysicalDevice = std::make_unique<DynamicPhysicalDevice>();
	}

	void VulkanDRHI::clean()
	{
        VulkanSwapChain::cleanSwapChain(&_device, &_swapChainFramebuffers, &_swapChainImageViews, &_swapChain);

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

	void VulkanDRHI::initialize(bool supportRayTracing)
	{   
		createInstance(&_instance);
		createSurface(&_surface, &_instance, _platformInfo);
		
        pickPhysicalDevice(&_physicalDevice, &_instance, 0);
		pickGraphicQueueFamily(&_physicalDevice, (uint32_t)-1);
        _virtualPhysicalDevice->internalID = _physicalDevice;
		createLogicalDevice(&_device, &_physicalDevice, &_graphicQueue, &_presentQueue, &_surface, &_queueFamilyIndices, supportRayTracing);
        _virtualDevice->internalID = _device;

        VulkanSwapChain::createSwapChain(&_swapChain, &_physicalDevice, &_device, &_surface, _platformInfo.window, &_swapChainImages, &_swapChainImageFormat, &_swapChainExtent, true);
        VulkanSwapChain::createImageViews(&_device, &_swapChainImageViews, &_swapChainImages, &_swapChainImageFormat);
        VulkanSwapChain::createDepthStencil(&_depthStencil, _depthFormat, _swapChainExtent.width, _swapChainExtent.height, VK_SAMPLE_COUNT_1_BIT, &_device, &_physicalDevice);

        VulkanPipeline::createPipelineCache(&_pipelineCache, &_device);

        createSemaphore(&_semaphores, &_device);

        createSynchronizationPrimitives(&_waitFences, MAX_FRAMES_IN_FLIGHT, &_device);

        initializeSubmitInfo();

        initializeOffscreenSemaphore();

        _prepare = true;
	}

    void VulkanDRHI::frameOnTick(std::vector<std::function<void()>> recreatefuncs, std::vector<DynamicCommandBuffer>* offscreenCommandBuffers, std::vector<DynamicCommandBuffer>* presentCommandBuffers)
    {
        // if need to stop rendering
        if ((_viewPortWidth <= 0) || (_viewPortHeight <= 0))
        {
            _waitForRendering = true;
            _lastWaitingState = _waitForRendering;
            return;
        }

        if (_lastWaitingState != _waitForRendering)
        {
            recreate(recreatefuncs);
            _lastWaitingState = _waitForRendering;
        }

        // prepare frame
        prepareFrame(recreatefuncs);

        std::vector<VkCommandBuffer> vkOffsceenCommandBuffers{};
        vkOffsceenCommandBuffers.resize(offscreenCommandBuffers->size());

        for (uint32_t i = 0; i < vkOffsceenCommandBuffers.size(); ++i)
        {
            vkOffsceenCommandBuffers[i] = (*offscreenCommandBuffers)[i].getVulkanCommandBuffer();
        }

        // submit offscreen rendering task
        _submitInfo.pWaitSemaphores = &_semaphores.presentComplete;
        _submitInfo.pSignalSemaphores = &_offscreenSemaphore;
        _submitInfo.commandBufferCount = vkOffsceenCommandBuffers.size();
        _submitInfo.pCommandBuffers = vkOffsceenCommandBuffers.data();

        if (vkQueueSubmit(_graphicQueue, 1, &_submitInfo, _waitFences[_currentFrame]) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to submit queue");
        }

        _submitInfo.pWaitSemaphores = &_offscreenSemaphore;
        _submitInfo.pSignalSemaphores = &_semaphores.renderComplete;

        // submit swapchain rendering task
        std::vector<VkCommandBuffer> vkCommandBuffers{};
        vkCommandBuffers.resize(presentCommandBuffers->size());

        for (uint32_t i = 0; i < vkCommandBuffers.size(); ++i)
        {
            vkCommandBuffers[i] = (*presentCommandBuffers)[i].getVulkanCommandBuffer();
        }
        _submitInfo.commandBufferCount = vkCommandBuffers.size();
        _submitInfo.pCommandBuffers = vkCommandBuffers.data();
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

        vkDestroySwapchainKHR(_device, _swapChain, nullptr);

        VulkanSwapChain::createSwapChain(&_swapChain, &_physicalDevice, &_device, &_surface, _platformInfo.window, &_swapChainImages, &_swapChainImageFormat, &_swapChainExtent, false);
        VulkanSwapChain::createImageViews(&_device, &_swapChainImageViews, &_swapChainImages, &_swapChainImageFormat);

        vkDestroyImageView(_device, _depthStencil.view, nullptr);
        vkDestroyImage(_device, _depthStencil.image, nullptr);
        vkFreeMemory(_device, _depthStencil.memory, nullptr);

        VulkanSwapChain::createDepthStencil(&_depthStencil, _depthFormat, _swapChainExtent.width, _swapChainExtent.height, VK_SAMPLE_COUNT_1_BIT ,&_device, &_physicalDevice);

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
        auto result = VulkanSwapChain::queuePresent(&_graphicQueue, &_swapChain, &_currentFrame, &_semaphores.renderComplete);
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

    void VulkanDRHI::initializeSubmitInfo()
    {
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

    void VulkanDRHI::initializeOffscreenSemaphore()
    {
        VkSemaphoreCreateInfo semaphoreCreateInfo{};
        semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        if (vkCreateSemaphore(_device, &semaphoreCreateInfo, nullptr, &_offscreenSemaphore) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to crete Semaphore");
        }
    }
}