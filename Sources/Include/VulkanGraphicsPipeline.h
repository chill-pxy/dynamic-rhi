#pragma once

#include"InterfaceType.h"

namespace DRHI
{
    static constexpr uint32_t SHADER_MODULE_COUNT = 2;
    static constexpr uint32_t VERTEX_ATTRIBUTE_COUNT = 16;

    //// The pipeline key is a POD that represents all currently bound states that form the immutable
    //// VkPipeline object. The size:offset comments below are expressed in bytes.
    struct PipelineKey {                                                          // size : offset
        VkShaderModule shaders[SHADER_MODULE_COUNT];                              //  16  : 0
        VkRenderPass renderPass;                                                  //  8   : 16
        //uint16_t topology;                                                        //  2   : 24
        //uint16_t subpassIndex;                                                    //  2   : 26
        //VertexInputAttributeDescription vertexAttributes[VERTEX_ATTRIBUTE_COUNT]; //  128 : 28
        //VertexInputBindingDescription vertexBuffers[VERTEX_ATTRIBUTE_COUNT];      //  128 : 156
        //RasterState rasterState;                                                  //  16  : 284
        //uint32_t padding;                                                         //  4   : 300
        //VkPipelineLayout layout;                                                  //  8   : 304
    };


}