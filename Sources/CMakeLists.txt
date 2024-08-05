cmake_minimum_required(VERSION 3.26.4 FATAL_ERROR)

set(TARGET_NAME dynamic_rhi)

project(${TARGET_NAME})

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SOURCE_FILES "*.cpp")
file(GLOB_RECURSE HEADER_FILES "*.h")

source_group("${CMAKE_CURRENT_SOURCE_DIR}" FILES ${HEADER_FILES} ${SOURCE_FILES})

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
add_library(${TARGET_NAME} SHARED ${HEADER_FILES} ${SOURCE_FILES})

set_target_properties(${TARGET_NAME} PROPERTIES OUTPUT_NAME "drhi")

target_link_libraries(${TARGET_NAME} PUBLIC
    glfw
    glm
    stb
    volk
)

include_directories(${THIRD_PARTY_DIR})

#---------------------------------#
#------------配置文件-------------#
#---------------------------------#
set(content " ")
set(codeDefine "undefine")
file(READ ${PROJECT_SOURCE_DIR}/config.txt content)
if(content STREQUAL "vk")
    set(codeDefine "#define VULKAN_IMPLEMENTATION")
endif()

if(content STREQUAL "dx12")
    set(codeDefine "#define DIRECT3D12_IMPLEMENTATION")
endif()

if(content STREQUAL "undefine")
    message(FATAL_ERROR "没有定义图形API")
endif()

file(WRITE ${PROJECT_SOURCE_DIR}/Include/InterfaceConfig.h "${codeDefine}" )
add_definitions(-DINCLUDE_GENERATED_HEADER)

#---------------------------------#
#------复制dll及lib到库目录-------#
#---------------------------------#
add_custom_command(
    TARGET ${TARGET_NAME} POST_BUILD # 对于库则使用对应的库名
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/drhi.dll 
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/../drhi/

    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/drhi.lib 
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/../drhi/
)