
file(GLOB_RECURSE MATERIALEDITOR_SOURCES CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)
file(GLOB_RECURSE MATERIALEDITOR_HEADERS CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/*.inl
)
file(GLOB_RECURSE MATERIALEDITOR_MODELS CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/Resources/*.obj
)

add_executable(MaterialEditor WIN32 ${MATERIALEDITOR_SOURCES} ${MATERIALEDITOR_HEADERS})

target_include_directories(MaterialEditor PRIVATE ${CMAKE_SOURCE_DIR}/MaterialEditor)

target_link_libraries(MaterialEditor PRIVATE RenderCore)

target_compile_features(MaterialEditor PRIVATE cxx_std_20)

target_sources(MaterialEditor PRIVATE ${MATERIALEDITOR_MODELS})

set_source_files_properties(${MATERIALEDITOR_MODELS} PROPERTIES
    HEADER_FILE_ONLY TRUE
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES
    ${MATERIALEDITOR_SOURCES}
    ${MATERIALEDITOR_HEADERS}
    ${MATERIALEDITOR_MODELS}
)

if (MSVC)
    target_compile_definitions(MaterialEditor PRIVATE UNICODE _UNICODE)
endif()

add_custom_command(TARGET MaterialEditor POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/Resources
        $<TARGET_FILE_DIR:MaterialEditor>
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/Shaders
        $<TARGET_FILE_DIR:MaterialEditor>/shaders
)