project(nikte)

set(EXAMPLE_SOURCES main.cpp
                    game.cpp)
add_executable(${PROJECT_NAME} ${EXAMPLE_SOURCES})
target_link_libraries(${PROJECT_NAME} PRIVATE ${STELLA_LIBRARY_NAME} editor::editor)

if (WIN32)
    # Copy assets folder
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${PROJECT_SOURCE_DIR}/assets"
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets)
    
    # Copy scripts folder
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${PROJECT_SOURCE_DIR}/scripts"
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/scripts)
    
    # Copy DLLS
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/lib/dll"
        $<TARGET_FILE_DIR:${PROJECT_NAME}>)
else()
    # Copy assets folder
    file(COPY assets DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

    # Copy scripts folder
    file(COPY scripts DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
endif()