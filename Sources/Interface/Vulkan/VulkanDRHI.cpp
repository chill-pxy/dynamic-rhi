#include<stdexcept>

#include"../../Include/Vulkan/VulkanDRHI.h"

namespace DRHI
{
	VulkanDRHI::VulkanDRHI(RHICreatInfo createInfo)
	{
		_glfwWindowCreateInfo = createInfo.glfwWindowCreateInfo;
	}

	void VulkanDRHI::clean()
	{
        cleanSwapChain(&_device, &_swapChainFramebuffers, &_swapChainImageViews, &_swapChain);

        //vkDestroyPipeline(_device, _graphicsPipeline, nullptr);
        //vkDestroyPipelineLayout(_device, _pipelineLayout, nullptr);
        //vkDestroyRenderPass(device, renderPass, nullptr);

        /*for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
            vkDestroyBuffer(device, uniformBuffers[i], nullptr);
            vkFreeMemory(device, uniformBuffersMemory[i], nullptr);
        }*/

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

        vkDestroyDevice(_device, nullptr);

        //if (enableValidationLayers) {
        //    DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
        //}

        vkDestroySurfaceKHR(_instance, _surface, nullptr);
        vkDestroyInstance(_instance, nullptr);

        cleanVulkanGlfwWindow(_glfwWindow);
	}

	void VulkanDRHI::initialize()
	{
		_glfwWindow = createGlfwWindow(&_extensions, _glfwWindowCreateInfo);
		createInstance(&_instance, _extensions);
		createSurface(&_surface, &_instance, _glfwWindow);
		pickPhysicalDevice(&_physicalDevice, &_instance, 0);
		pickGraphicQueueFamily(&_physicalDevice, (uint32_t)-1);
		createLogicalDevice(&_device, &_physicalDevice, &_graphicQueue, &_presentQueue, &_surface, &_queueFamilyIndices);
		createSwapChain(&_swapChain, &_physicalDevice, &_device, &_surface, _glfwWindow, _swapChainImages, &_swapChainImageFormat, _swapChainExtent);
		createImageViews(&_device, _swapChainImageViews, _swapChainImages, &_swapChainImageFormat);
		createCommandPool(&_commandPool, &_device, _queueFamilyIndices);
		createDescriptorSetLayout(&_descriptorSetLayout, &_device);
		createDescriptorPool(&_descriptorPool, &_device);
        createCommandBuffers(&_commandBuffers, &_commandPool, &_device);
	}

    void VulkanDRHI::createPipeline(VulkanPipelineCreateInfo info)
    {
        createGraphicsPipeline(&_graphicsPipeline, &_pipelineLayout, info, &_device,& _descriptorSetLayout, &_swapChainImageFormat);
    }
}