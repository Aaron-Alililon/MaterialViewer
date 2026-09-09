
file(GLOB_RECURSE MATERIALEDITOR_SOURCES CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)
file(GLOB_RECURSE MATERIALEDITOR_HEADERS CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/*.inl
)

add_executable(MaterialEditor WIN32 ${MATERIALEDITOR_SOURCES} ${MATERIALEDITOR_HEADERS})

target_include_directories(MaterialEditor PRIVATE ${CMAKE_SOURCE_DIR}/MaterialEditor)

target_link_libraries(MaterialEditor PRIVATE RenderCore)

target_compile_features(MaterialEditor PRIVATE cxx_std_20)

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