#include<algorithm>
#include<limits>

#include"../../Include/Vulkan/VulkanSwapChain.h"
#include"../../Include/Vulkan/VulkanPhysicalDevice.h"
#include"../../Include/Vulkan/VulkanImage.h"

namespace DRHI
{
    void createSwapChain(VkSwapchainKHR* swapChain, VkPhysicalDevice* physicalDevice, VkDevice* device, VkSurfaceKHR* surface, HWND window,
        std::vector<VkImage>* swapChainImages, VkFormat* swapChainImageFormat, VkExtent2D* swapChainExtent, uint32_t* viewPortWidth, uint32_t* viewPortHeight)
    {
        VkSwapchainKHR oldSwapChain = *swapChain;

        SwapChainSupportDetails swapChainSupport = querySwapChainSupport(physicalDevice, surface);

        VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
        VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
        VkExtent2D extent{};
        if ((!viewPortWidth) || (!viewPortHeight))
        {
            extent = chooseSwapExtent(window, swapChainSupport.capabilities);
        }
        else
        {
            extent.width = *viewPortWidth;
            extent.height = *viewPortHeight;
        }

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
        createInfo.oldSwapchain = oldSwapChain;

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
        viewPortWidth = &extent.width;
        viewPortHeight = &extent.height;
    }

    void createImageViews(VkDevice* device, std::vector<VkImageView>* swapChainImageViews, std::vector<VkImage>* swapChainImages, VkFormat* swapChainImageFormat)
    {
        swapChainImageViews->resize(swapChainImages->size());

        for (uint32_t i = 0; i < swapChainImages->size(); i++)
        {
            VkImage scImages = (*swapChainImages)[i];
            (*swapChainImageViews)[i] = VulkanImage::createImageView(device, &scImages, *swapChainImageFormat, VK_IMAGE_ASPECT_COLOR_BIT);
        }
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

    VkExtent2D chooseSwapExtent(HWND hwnd, const VkSurfaceCapabilitiesKHR& capabilities)
    {
        if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) 
        {
            return capabilities.currentExtent;
        }
        else
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

            actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
            actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

            return actualExtent;
        }
    }

    void cleanSwapChain(VkDevice* device, std::vector<VkFramebuffer>* swapChainFramebuffers, std::vector<VkImageView>* swapChainImageViews, VkSwapchainKHR* swapChain)
    {
        for (auto framebuffer : *swapChainFramebuffers) {
            vkDestroyFramebuffer(*device, framebuffer, nullptr);
        }

        for (auto imageView : *swapChainImageViews) {
            vkDestroyImageView(*device, imageView, nullptr);
        }

        vkDestroySwapchainKHR(*device, *swapChain, nullptr);
    }

    VkResult queuePresent(VkQueue* queue, VkSwapchainKHR* swapChain, uint32_t* imageIndex, VkSemaphore* waitSemaphore)
    {
        VkPresentInfoKHR presentInfo = {};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        presentInfo.pNext = NULL;
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = swapChain;
        presentInfo.pImageIndices = imageIndex;
        // Check if a wait semaphore has been specified to wait for before presenting the image
        if (waitSemaphore != VK_NULL_HANDLE)
        {
            presentInfo.pWaitSemaphores = waitSemaphore;
            presentInfo.waitSemaphoreCount = 1;
        }
        
        return vkQueuePresentKHR(*queue, &presentInfo);
    }

    void createDepthStencil(DepthStencil* depthStencil, VkFormat depthFormat, uint32_t width, uint32_t height, VkDevice* device, VkPhysicalDevice* physicalDevice)
    {
        VkImageCreateInfo imageCI{};
        imageCI.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        imageCI.imageType = VK_IMAGE_TYPE_2D;
        imageCI.format = depthFormat;
        imageCI.extent = { width, height, 1 };
        imageCI.mipLevels = 1;
        imageCI.arrayLayers = 1;
        imageCI.samples = VK_SAMPLE_COUNT_1_BIT;
        imageCI.tiling = VK_IMAGE_TILING_OPTIMAL;
        imageCI.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;

        if ((vkCreateImage(*device, &imageCI, nullptr, &depthStencil->image)) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create image");
        }
        VkMemoryRequirements memReqs{};
        vkGetImageMemoryRequirements(*device, depthStencil->image, &memReqs);

        VkMemoryAllocateInfo memAllloc{};
        memAllloc.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        memAllloc.allocationSize = memReqs.size;
        memAllloc.memoryTypeIndex = getMemoryType(physicalDevice, memReqs.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

        VkDeviceMemory depthMemory;

        if (vkAllocateMemory(*device, &memAllloc, nullptr, &depthMemory) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to allocate memory");
        }

        depthStencil->memory = depthMemory;

        if (vkBindImageMemory(*device, depthStencil->image, depthStencil->memory, 0) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to allocate memory");
        }

        VkImageViewCreateInfo imageViewCI{};
        imageViewCI.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        imageViewCI.viewType = VK_IMAGE_VIEW_TYPE_2D;
        imageViewCI.image = depthStencil->image;
        imageViewCI.format = depthFormat;
        imageViewCI.subresourceRange.baseMipLevel = 0;
        imageViewCI.subresourceRange.levelCount = 1;
        imageViewCI.subresourceRange.baseArrayLayer = 0;
        imageViewCI.subresourceRange.layerCount = 1;
        imageViewCI.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
        // Stencil aspect should only be set on depth + stencil formats (VK_FORMAT_D16_UNORM_S8_UINT..VK_FORMAT_D32_SFLOAT_S8_UINT
        if (depthFormat >= VK_FORMAT_D16_UNORM_S8_UINT) {
            imageViewCI.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
        }
        if (vkCreateImageView(*device, &imageViewCI, nullptr, &depthStencil->view) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create image view");
        }
    }

}