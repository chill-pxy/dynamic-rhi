#include<stdexcept>
#include<vector>

#include"../../Include/Vulkan/VulkanPipeline.h"
#include"../../Include/CoreFunction.h"

namespace DRHI
{
    namespace VulkanPipeline
    {
        void createPipelineLayout(DynamicPipelineLayout* pipelineLayout, DynamicPipelineLayoutCreateInfo* createInfo, VkDevice* device)
        {
            VkPipelineLayout vkpipelineLayout{};

            VkDescriptorSetLayout vkdescriptorSetLayout = createInfo->pSetLayouts->getVulkanDescriptorSetLayout();

            VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
            pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
            pipelineLayoutInfo.setLayoutCount = createInfo->setLayoutCount;
            pipelineLayoutInfo.pSetLayouts = &vkdescriptorSetLayout;

            if (createInfo->pushConstantRangeCount > 0)
            {
                VkPushConstantRange push{};
                push.offset = createInfo->pPushConstantRanges->offset;
                push.size = createInfo->pPushConstantRanges->size;
                push.stageFlags = (VkShaderStageFlagBits)createInfo->pPushConstantRanges->stageFlags;
                pipelineLayoutInfo.pPushConstantRanges = &push;
                pipelineLayoutInfo.pushConstantRangeCount = createInfo->pushConstantRangeCount;
            }

            if (vkCreatePipelineLayout(*device, &pipelineLayoutInfo, nullptr, &vkpipelineLayout) != VK_SUCCESS) 
            {
                throw std::runtime_error("failed to create pipeline layout!");
            }

            pipelineLayout->internalID = vkpipelineLayout;
            createInfo->pSetLayouts->internalID = vkdescriptorSetLayout;
        }

        void createGraphicsPipeline(VkPipeline* graphicsPipeline, VkPipelineLayout* pipelineLayout, VkPipelineCache* pipelineCache,
            VulkanPipelineCreateInfo createInfo, VkDevice* device, VkFormat* swapChainImageFormat,
            VkVertexInputBindingDescription bindingDescription, std::vector<VkVertexInputAttributeDescription> attributeDescriptions)
        {
            //----------------------------------shader state----------------------------------
            VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
            vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
            vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
            vertShaderStageInfo.module = createInfo.vertexShader;
            vertShaderStageInfo.pName = "main";

            VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
            fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
            fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
            fragShaderStageInfo.module = createInfo.fragmentShader;
            fragShaderStageInfo.pName = "main";

            VkPipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageInfo,fragShaderStageInfo };
            //-------------------------------------------------------------------------------



            //----------------------------------VertexInput---------------------------------
            VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
            vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

            vertexInputInfo.vertexBindingDescriptionCount = 1;
            vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
            vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
            vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

            VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
            inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
            inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
            inputAssembly.primitiveRestartEnable = VK_FALSE;
            //----------------------------------------------------------------------------------



            VkPipelineViewportStateCreateInfo viewportState{};
            viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
            viewportState.viewportCount = 1;
            viewportState.scissorCount = 1;

            VkPipelineRasterizationStateCreateInfo rasterizer{};
            rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
            rasterizer.depthClampEnable = VK_FALSE;
            rasterizer.rasterizerDiscardEnable = VK_FALSE;
            rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
            rasterizer.lineWidth = 1.0f;
            rasterizer.cullMode = VK_CULL_MODE_NONE;
            rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
            rasterizer.depthBiasEnable = VK_FALSE;

            VkPipelineMultisampleStateCreateInfo multisampling{};
            multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
            multisampling.sampleShadingEnable = VK_FALSE;
            multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

            VkPipelineDepthStencilStateCreateInfo depthStencil{};
            depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
            depthStencil.depthTestEnable = VK_TRUE;
            depthStencil.depthWriteEnable = VK_TRUE;
            depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
            depthStencil.depthBoundsTestEnable = VK_FALSE;
            depthStencil.stencilTestEnable = VK_FALSE;

            VkPipelineColorBlendAttachmentState colorBlendAttachment{};
            colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
            colorBlendAttachment.blendEnable = VK_FALSE;

            VkPipelineColorBlendStateCreateInfo colorBlending{};
            colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
            colorBlending.logicOpEnable = VK_FALSE;
            colorBlending.logicOp = VK_LOGIC_OP_COPY;
            colorBlending.attachmentCount = 1;
            colorBlending.pAttachments = &colorBlendAttachment;
            colorBlending.blendConstants[0] = 0.0f;
            colorBlending.blendConstants[1] = 0.0f;
            colorBlending.blendConstants[2] = 0.0f;
            colorBlending.blendConstants[3] = 0.0f;

            std::vector<VkDynamicState> dynamicStates = {
                VK_DYNAMIC_STATE_VIEWPORT,
                VK_DYNAMIC_STATE_SCISSOR,
            };
            VkPipelineDynamicStateCreateInfo dynamicState{};
            dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
            dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
            dynamicState.pDynamicStates = dynamicStates.data();

            //VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
            //pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
            //pipelineLayoutInfo.setLayoutCount = 1;
            //pipelineLayoutInfo.pSetLayouts = descriptorSetlayout;

            //if (vkCreatePipelineLayout(*device, &pipelineLayoutInfo, nullptr, pipelineLayout) != VK_SUCCESS) {
            //    throw std::runtime_error("failed to create pipeline layout!");
            //}

            VkPipelineRenderingCreateInfoKHR pipelineRenderingCreateInfo{};
            pipelineRenderingCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO_KHR;
            pipelineRenderingCreateInfo.colorAttachmentCount = 1;
            pipelineRenderingCreateInfo.pColorAttachmentFormats = swapChainImageFormat;
            pipelineRenderingCreateInfo.depthAttachmentFormat = VK_FORMAT_D32_SFLOAT_S8_UINT;
            pipelineRenderingCreateInfo.stencilAttachmentFormat = VK_FORMAT_D32_SFLOAT_S8_UINT;

            VkPipelineTessellationStateCreateInfo pipelineTessellationStateCreateInfo{};
            pipelineTessellationStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO;
            pipelineTessellationStateCreateInfo.patchControlPoints = 3;

            VkGraphicsPipelineCreateInfo pipelineInfo{};
            pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
            pipelineInfo.pNext = &pipelineRenderingCreateInfo;
            pipelineInfo.stageCount = 2;
            pipelineInfo.pStages = shaderStages;
            pipelineInfo.pVertexInputState = &vertexInputInfo;
            pipelineInfo.pInputAssemblyState = &inputAssembly;
            pipelineInfo.pViewportState = &viewportState;
            pipelineInfo.pRasterizationState = &rasterizer;
            pipelineInfo.pMultisampleState = &multisampling;
            pipelineInfo.pDepthStencilState = &depthStencil;
            pipelineInfo.pColorBlendState = &colorBlending;
            pipelineInfo.pDynamicState = &dynamicState;
            pipelineInfo.layout = *pipelineLayout;
            pipelineInfo.renderPass = VK_NULL_HANDLE;
            pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
            pipelineInfo.pTessellationState = &pipelineTessellationStateCreateInfo;

            if (vkCreateGraphicsPipelines(*device, *pipelineCache, 1, &pipelineInfo, nullptr, graphicsPipeline) != VK_SUCCESS)
            {
                throw std::runtime_error("failed to create graphics pipeline!");
            }

            vkDestroyShaderModule(*device, createInfo.fragmentShader, nullptr);
            vkDestroyShaderModule(*device, createInfo.vertexShader, nullptr);
        }

        void createPipelineCache(VkPipelineCache* pipelineCache, VkDevice* device)
        {
            VkPipelineCacheCreateInfo pipelineCacheCreateInfo = {};
            pipelineCacheCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
            if (vkCreatePipelineCache(*device, &pipelineCacheCreateInfo, nullptr, pipelineCache))
            {
                throw std::runtime_error("failed to create pipeline cache");
            }
        }

        VkPipelineRenderingCreateInfoKHR getPipelineRenderingCreateInfo(VkFormat* swapChainImageFormat)
        {
            VkPipelineRenderingCreateInfoKHR pipelineRenderingCreateInfo{};
            pipelineRenderingCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO_KHR;
            pipelineRenderingCreateInfo.colorAttachmentCount = 1;
            pipelineRenderingCreateInfo.pColorAttachmentFormats = swapChainImageFormat;
            pipelineRenderingCreateInfo.depthAttachmentFormat = VK_FORMAT_D32_SFLOAT_S8_UINT;
            pipelineRenderingCreateInfo.stencilAttachmentFormat = VK_FORMAT_D32_SFLOAT_S8_UINT;

            return pipelineRenderingCreateInfo;
        }
    }
}