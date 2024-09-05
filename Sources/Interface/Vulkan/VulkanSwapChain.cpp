#include<algorithm>
#include<limits>

#include"../../Include/Vulkan/VulkanSwapChain.h"

namespace DRHI
{
    void createSwapChain(VkSwapchainKHR* swapChain, VkPhysicalDevice* physicalDevice, VkDevice* device, VkSurfaceKHR* surface, HWND window,
        std::vector<VkImage>* swapChainImages, VkFormat* swapChainImageFormat, VkExtent2D* swapChainExtent)
    {
        SwapChainSupportDetails swapChainSupport = querySwapChainSupport(physicalDevice, surface);

        VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
        VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
        VkExtent2D extent = chooseSwapExtent(window);

        uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
        if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount)
        {
            imageCount = swapChainSupport.capabilities.maxImageCount;
        }

        VkSwapchainCreateInfoKHR createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        createInfo.surface = *surface;

        createInfo.minImageCount = imageCount;
        createInfo.imageFormat = surfaceFormat.format;
        createInfo.imageColorSpace = surfaceFormat.colorSpace;
        createInfo.imageExtent = extent;
        createInfo.imageArrayLayers = 1;
        createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

        QueueFamilyIndices indices = findQueueFamilies(*physicalDevice, *surface);
        uint32_t queueFamilyIndices[] = { indices.graphicsFamily.value(), indices.presentFamily.value() };

        if (indices.graphicsFamily != indices.presentFamily)
        {
            createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            createInfo.queueFamilyIndexCount = 2;
            createInfo.pQueueFamilyIndices = queueFamilyIndices;
        }
        else
        {
            createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        }

        createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
        createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        createInfo.presentMode = presentMode;
        createInfo.clipped = VK_TRUE;

        if (vkCreateSwapchainKHR(*device, &createInfo, nullptr, swapChain) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create swap chain!");
        }

        vkGetSwapchainImagesKHR(*device, *swapChain, &imageCount, nullptr);
        swapChainImages->resize(imageCount);
        vkGetSwapchainImagesKHR(*device, *swapChain, &imageCount, swapChainImages->data());

        *swapChainImageFormat = surfaceFormat.format;
        *swapChainExtent = extent;
    }

    void createImageViews(VkDevice* device, std::vector<VkImageView>* swapChainImageViews, std::vector<VkImage>* swapChainImages, VkFormat* swapChainImageFormat)
    {
        swapChainImageViews->resize(swapChainImages->size());

        for (uint32_t i = 0; i < swapChainImages->size(); i++)
        {
            VkImage scImages = (*swapChainImages)[i];
            (*swapChainImageViews)[i] = createImageView(device, &scImages, *swapChainImageFormat, VK_IMAGE_ASPECT_COLOR_BIT);
        }
    }

    VkImageView createImageView(VkDevice* device, VkImage* image, VkFormat format, VkImageAspectFlags aspectFlags)
    {
        VkImageViewCreateInfo viewInfo{};
        viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        viewInfo.image = *image;
        viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        viewInfo.format = format;
        viewInfo.subresourceRange.aspectMask = aspectFlags;
        viewInfo.subresourceRange.baseMipLevel = 0;
        viewInfo.subresourceRange.levelCount = 1;
        viewInfo.subresourceRange.baseArrayLayer = 0;
        viewInfo.subresourceRange.layerCount = 1;

        VkImageView imageView;
        if (vkCreateImageView(*device, &viewInfo, nullptr, &imageView) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create image view!");
        }

        return imageView;
    }

    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice* device, VkSurfaceKHR* surface)
    {
        SwapChainSupportDetails details;

        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(*device, *surface, &details.capabilities);

        uint32_t formatCount;
        vkGetPhysicalDeviceSurfaceFormatsKHR(*device, *surface, &formatCount, nullptr);

        if (formatCount != 0)
        {
            details.formats.resize(formatCount);
            vkGetPhysicalDeviceSurfaceFormatsKHR(*device, *surface, &formatCount, details.formats.data());
        }

        uint32_t presentModeCount;
        vkGetPhysicalDeviceSurfacePresentModesKHR(*device, *surface, &presentModeCount, nullptr);

        if (presentModeCount != 0)
        {
            details.presentModes.resize(presentModeCount);
            vkGetPhysicalDeviceSurfacePresentModesKHR(*device, *surface, &presentModeCount, details.presentModes.data());
        }

        return details;
    }

    VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
    {
        for (const auto& availableFormat : availableFormats)
        {
            if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
            {
                return availableFormat;
            }
        }

        return availableFormats[0];
    }

    VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes)
    {
        for (const auto& availablePresentMode : availablePresentModes)
        {
            if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
            {
                return availablePresentMode;
            }
        }

        return VK_PRESENT_MODE_FIFO_KHR;
    }

    VkExtent2D chooseSwapExtent(HWND hwnd)
    {
        RECT rect;
        GetWindowRect(hwnd, &rect);
        int width = rect.right - rect.left;
        int height = rect.bottom - rect.top;

        VkExtent2D actualExtent =
        {
            static_cast<uint32_t>(width),
            static_cast<uint32_t>(height)
        };

        actualExtent.width = std::clamp(actualExtent.width, (uint32_t)0, (uint32_t)GetSystemMetrics(SM_CXSCREEN));
        actualExtent.height = std::clamp(actualExtent.height, (uint32_t)0, (uint32_t)GetSystemMetrics(SM_CYSCREEN));

        return actualExtent;
    }

    void cleanSwapChain(VkDevice* device, std::vector<VkFramebuffer>* swapChainFramebuffers, std::vector<VkImageView>* swapChainImageViews, VkSwapchainKHR* swapChain)
    {
        //vkDestroyImageView(device, depthImageView, nullptr);
        //vkDestroyImage(device, depthImage, nullptr);
        //vkFreeMemory(device, depthImageMemory, nullptr);

        for (auto framebuffer : *swapChainFramebuffers) {
            vkDestroyFramebuffer(*device, framebuffer, nullptr);
        }

        for (auto imageView : *swapChainImageViews) {
            vkDestroyImageView(*device, imageView, nullptr);
        }

        vkDestroySwapchainKHR(*device, *swapChain, nullptr);
    }

}