
file(GLOB_RECURSE MATERIALVIEWER_SOURCES CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)
file(GLOB_RECURSE MATERIALVIEWER_HEADERS CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/*.inl
)
file(GLOB_RECURSE MATERIALVIEWER_MODELS CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/Resources/*.obj
)

file(GLOB_RECURSE MATERIALVIEWER_SHADERS CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/Shaders/*.hlsl
    ${CMAKE_CURRENT_SOURCE_DIR}/Shaders/*.hlsli
)

add_executable(MaterialViewer WIN32 ${MATERIALVIEWER_SOURCES} ${MATERIALVIEWER_HEADERS})

target_include_directories(MaterialViewer PRIVATE ${CMAKE_SOURCE_DIR}/MaterialViewer)

target_link_libraries(MaterialViewer PRIVATE RenderCore)

target_compile_features(MaterialViewer PRIVATE cxx_std_20)

target_sources(MaterialViewer PRIVATE ${MATERIALVIEWER_MODELS} ${MATERIALVIEWER_SHADERS})

set_source_files_properties(${MATERIALVIEWER_MODELS} ${MATERIALVIEWER_SHADERS} PROPERTIES
    HEADER_FILE_ONLY TRUE
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES
    ${MATERIALVIEWER_SOURCES}
    ${MATERIALVIEWER_HEADERS}
    ${MATERIALVIEWER_MODELS}
    ${MATERIALVIEWER_SHADERS}
)

if (MSVC)
    target_compile_definitions(MaterialViewer PRIVATE UNICODE _UNICODE)
endif()

set(SHADER_STAMP ${CMAKE_CURRENT_BINARY_DIR}/shaders.stamp)

add_custom_command(
    OUTPUT ${SHADER_STAMP}
    COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
        ${CMAKE_CURRENT_SOURCE_DIR}/Shaders
        $<TARGET_FILE_DIR:MaterialViewer>/shaders
    COMMAND ${CMAKE_COMMAND} -E touch ${SHADER_STAMP}
    DEPENDS ${MATERIALVIEWER_SHADERS}
    COMMENT "Copying shaders"
)

add_custom_command(TARGET MaterialViewer POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
        ${CMAKE_CURRENT_SOURCE_DIR}/Resources
        $<TARGET_FILE_DIR:MaterialViewer>
)

add_custom_target(CopyShaders ALL DEPENDS ${SHADER_STAMP})
add_dependencies(MaterialViewer CopyShaders)