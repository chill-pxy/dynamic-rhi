#pragma once

#include<vector>
#include<stdexcept>

#include"../../Include/InterfaceType.h"
#include"VulkanQueueFamily.h"

namespace DRHI
{
    struct SwapChainSupportDetails
    {
        VkSurfaceCapabilitiesKHR        capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR>   presentModes;
    };

    void createSwapChain(VkSwapchainKHR* swapChain, VkPhysicalDevice* physicalDevice, VkDevice* device, VkSurfaceKHR* surface, GLFWwindow* window,
        std::vector<VkImage> swapChainImages, VkFormat* swapChainImageFormat, VkExtent2D swapChainExtent);

    void createImageViews(VkDevice* device, std::vector<VkImageView> swapChainImageViews, std::vector<VkImage> swapChainImages, VkFormat* swapChainImageFormat);

    VkImageView createImageView(VkDevice* device, VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);

    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice* device, VkSurfaceKHR* surface);

    VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);

    VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);

    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, GLFWwindow* window);
}

