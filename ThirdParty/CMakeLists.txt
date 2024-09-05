set(third_party_folder "ThirdParty")

if(NOT TARGET glm)
    option(BUILD_STATIC_LIBS "" ON)
    option(BUILD_TESTING "" OFF)
    option(GLM_TEST_ENABLE "" OFF)
    add_subdirectory(glm)
    set_target_properties(glm_static PROPERTIES FOLDER ${third_party_folder}/glm)
endif()

if(NOT TARGET stb)
    include(stb.cmake)
    set_target_properties(stb PROPERTIES FOLDER ${third_party_folder}/stb)
endif()


if(NOT TARGET volk)
    add_subdirectory(volk)
endif()
