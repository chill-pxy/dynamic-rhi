#include "../../Include/InterfaceConfig.h"
#include "../../Include/GraphicsPipeline.h"

#ifdef VULKAN_IMPLEMENTATION

namespace DRHI
{
	void GraphicsPipeline::createGraphicsPipeline()
	{
        //mPipelineRequirements.shaders[0]
        //mPipelineRequirements.layout

//        VkPipelineShaderStageCreateInfo shaderStages[SHADER_MODULE_COUNT];
//        shaderStages[0] = VkPipelineShaderStageCreateInfo{};
//        shaderStages[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
//        shaderStages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
//        shaderStages[0].pName = "main";
//
//        shaderStages[1] = VkPipelineShaderStageCreateInfo{};
//        shaderStages[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
//        shaderStages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
//        shaderStages[1].pName = "main";
//
//        VkPipelineColorBlendAttachmentState colorBlendAttachment{};
//        VkPipelineColorBlendStateCreateInfo colorBlendState;
//        colorBlendState = VkPipelineColorBlendStateCreateInfo{};
//        colorBlendState.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
//        colorBlendState.attachmentCount = 1;
//        colorBlendState.pAttachments = &colorBlendAttachment;
//
//        // If we reach this point, we need to create and stash a brand new pipeline object.
//        shaderStages[0].module = _pipeLineRequirements.shaders[0];
//        shaderStages[1].module = _pipeLineRequirements.shaders[1];
//
//        // Expand our size-optimized structs into the proper Vk structs.
//        uint32_t numVertexAttribs = 0;
//        uint32_t numVertexBuffers = 0;
//        VkVertexInputAttributeDescription vertexAttributes[VERTEX_ATTRIBUTE_COUNT];
//        VkVertexInputBindingDescription vertexBuffers[VERTEX_ATTRIBUTE_COUNT];
//        for (uint32_t i = 0; i < VERTEX_ATTRIBUTE_COUNT; i++) {
//            if (_pipeLineRequirements.vertexAttributes[i].format > 0) {
//                vertexAttributes[numVertexAttribs] = _pipeLineRequirements.vertexAttributes[i];
//                numVertexAttribs++;
//            }
//            if (_pipeLineRequirements.vertexBuffers[i].stride > 0) {
//                vertexBuffers[numVertexBuffers] = _pipeLineRequirements.vertexBuffers[i];
//                numVertexBuffers++;
//            }
//        }
//
//        VkPipelineVertexInputStateCreateInfo vertexInputState = {};
//        vertexInputState.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
//        vertexInputState.vertexBindingDescriptionCount = numVertexBuffers;
//        vertexInputState.pVertexBindingDescriptions = vertexBuffers;
//        vertexInputState.vertexAttributeDescriptionCount = numVertexAttribs;
//        vertexInputState.pVertexAttributeDescriptions = vertexAttributes;
//
//        VkPipelineInputAssemblyStateCreateInfo inputAssemblyState = {};
//        inputAssemblyState.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
//        inputAssemblyState.topology = (VkPrimitiveTopology)_pipeLineRequirements.topology;
//
//        VkPipelineViewportStateCreateInfo viewportState = {};
//        viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
//        viewportState.viewportCount = 1;
//        viewportState.scissorCount = 1;
//
//        VkDynamicState dynamicStateEnables[] = {
//            VK_DYNAMIC_STATE_VIEWPORT,
//            VK_DYNAMIC_STATE_SCISSOR,
//        };
//        VkPipelineDynamicStateCreateInfo dynamicState = {};
//        dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
//        dynamicState.pDynamicStates = dynamicStateEnables;
//        dynamicState.dynamicStateCount = 2;
//
//        const bool hasFragmentShader = shaderStages[1].module != VK_NULL_HANDLE;
//
//        VkGraphicsPipelineCreateInfo pipelineCreateInfo = {};
//        pipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
//        pipelineCreateInfo.layout = _pipeLineRequirements.layout;
//        pipelineCreateInfo.renderPass = _pipeLineRequirements.renderPass;
//        pipelineCreateInfo.subpass = _pipeLineRequirements.subpassIndex;
//        pipelineCreateInfo.stageCount = hasFragmentShader ? SHADER_MODULE_COUNT : 1;
//        pipelineCreateInfo.pStages = shaderStages;
//        pipelineCreateInfo.pVertexInputState = &vertexInputState;
//        pipelineCreateInfo.pInputAssemblyState = &inputAssemblyState;
//
//        VkPipelineRasterizationStateCreateInfo vkRaster = {};
//        vkRaster.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
//        pipelineCreateInfo.pRasterizationState = &vkRaster;
//
//        VkPipelineMultisampleStateCreateInfo vkMs = {};
//        vkMs.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
//        pipelineCreateInfo.pMultisampleState = &vkMs;
//
//        VkPipelineDepthStencilStateCreateInfo vkDs = {};
//        vkDs.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
//        vkDs.front = vkDs.back = {
//                .failOp = VK_STENCIL_OP_KEEP,
//                .passOp = VK_STENCIL_OP_KEEP,
//                .depthFailOp = VK_STENCIL_OP_KEEP,
//                .compareOp = VK_COMPARE_OP_ALWAYS,
//                .compareMask = 0u,
//                .writeMask = 0u,
//                .reference = 0u,
//        };
//        pipelineCreateInfo.pDepthStencilState = &vkDs;
//
//        const auto& raster = _pipeLineRequirements.rasterState;
//
//        vkRaster.polygonMode = VK_POLYGON_MODE_FILL;
//        vkRaster.cullMode = raster.cullMode;
//        vkRaster.frontFace = raster.frontFace;
//        vkRaster.depthBiasEnable = raster.depthBiasEnable;
//        vkRaster.depthBiasConstantFactor = raster.depthBiasConstantFactor;
//        vkRaster.depthBiasClamp = 0.0f;
//        vkRaster.depthBiasSlopeFactor = raster.depthBiasSlopeFactor;
//        vkRaster.lineWidth = 1.0f;
//
//        vkMs.rasterizationSamples = (VkSampleCountFlagBits)raster.rasterizationSamples;
//        vkMs.sampleShadingEnable = VK_FALSE;
//        vkMs.minSampleShading = 0.0f;
//        vkMs.alphaToCoverageEnable = raster.alphaToCoverageEnable;
//        vkMs.alphaToOneEnable = VK_FALSE;
//
//        vkDs.depthTestEnable = VK_TRUE;
//        vkDs.depthWriteEnable = raster.depthWriteEnable;
//        vkDs.depthCompareOp = getCompareOp(raster.depthCompareOp);
//        vkDs.depthBoundsTestEnable = VK_FALSE;
//        vkDs.stencilTestEnable = VK_FALSE;
//        vkDs.minDepthBounds = 0.0f;
//        vkDs.maxDepthBounds = 0.0f;
//
//        pipelineCreateInfo.pColorBlendState = &colorBlendState;
//        pipelineCreateInfo.pViewportState = &viewportState;
//        pipelineCreateInfo.pDynamicState = &dynamicState;
//
//        // Filament assumes consistent blend state across all color attachments.
//        colorBlendState.attachmentCount = _pipeLineRequirements.rasterState.colorTargetCount;
//        for (auto& target : colorBlendAttachments) {
//            target.blendEnable = mPipelineRequirements.rasterState.blendEnable;
//            target.srcColorBlendFactor = mPipelineRequirements.rasterState.srcColorBlendFactor;
//            target.dstColorBlendFactor = mPipelineRequirements.rasterState.dstColorBlendFactor;
//            target.colorBlendOp = (VkBlendOp)mPipelineRequirements.rasterState.colorBlendOp;
//            target.srcAlphaBlendFactor = mPipelineRequirements.rasterState.srcAlphaBlendFactor;
//            target.dstAlphaBlendFactor = mPipelineRequirements.rasterState.dstAlphaBlendFactor;
//            target.alphaBlendOp = (VkBlendOp)mPipelineRequirements.rasterState.alphaBlendOp;
//            target.colorWriteMask = mPipelineRequirements.rasterState.colorWriteMask;
//        }
//
//        // There are no color attachments if there is no bound fragment shader.  (e.g. shadow map gen)
//        // TODO: This should be handled in a higher layer.
//        if (!hasFragmentShader) {
//            colorBlendState.attachmentCount = 0;
//        }
//
//        PipelineCacheEntry cacheEntry = {};
//
//#if FVK_ENABLED(FVK_DEBUG_SHADER_MODULE)
//        utils::slog.d << "vkCreateGraphicsPipelines with shaders = ("
//            << shaderStages[0].module << ", " << shaderStages[1].module << ")" << utils::io::endl;
//#endif
//        VkResult error = vkCreateGraphicsPipelines(mDevice, VK_NULL_HANDLE, 1, &pipelineCreateInfo,
//            VKALLOC, &cacheEntry.handle);
//        assert_invariant(error == VK_SUCCESS);
//        if (error != VK_SUCCESS) {
//            utils::slog.e << "vkCreateGraphicsPipelines error " << error << utils::io::endl;
//            return nullptr;
//        }
//
//        return &mPipelines.emplace(mPipelineRequirements, cacheEntry).first.value();
	}

}

#endif