#pragma once

#include<vector>
#include<stdexcept>

#include"../../Include/InterfaceType.h"
#include"VulkanQueueFamily.h"
#include"VulkanSurface.h"

namespace DRHI
{
    struct SwapChainSupportDetails
    {
        VkSurfaceCapabilitiesKHR        capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR>   presentModes;
    };

    /** @brief Default depth stencil attachment used by the default render pass */
    struct DepthStencil 
    {
        VkImage image;
        VkDeviceMemory memory;
        VkImageView view;
    };

    void createSwapChain(VkSwapchainKHR* swapChain, VkPhysicalDevice* physicalDevice, VkDevice* device, VkSurfaceKHR* surface, HWND window,
        std::vector<VkImage>* swapChainImages, VkFormat* swapChainImageFormat, VkExtent2D* swapChainExtent);

    void createImageViews(VkDevice* device, std::vector<VkImageView>* swapChainImageViews, std::vector<VkImage>* swapChainImages, VkFormat* swapChainImageFormat);

    VkImageView createImageView(VkDevice* device, VkImage* image, VkFormat format, VkImageAspectFlags aspectFlags);

    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice* device, VkSurfaceKHR* surface);

    VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);

    VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);

    VkExtent2D chooseSwapExtent(HWND window, const VkSurfaceCapabilitiesKHR& capabilities);

    void cleanSwapChain(VkDevice* device, std::vector<VkFramebuffer>* swapChainFramebuffers, std::vector<VkImageView>* swapChainImageViews, VkSwapchainKHR* swapChain);

    VkResult queuePresent(VkQueue* queue, VkSwapchainKHR* swapChain, uint32_t* imageIndex, VkSemaphore* waitSemaphore);

    void createDepthStencil(DepthStencil* depthStencil, VkFormat depthFormat, uint32_t width, uint32_t height, VkDevice* device, VkPhysicalDevice* physicalDevice);
}

