project(example)

set(EXAMPLE_SOURCES main.cpp)
add_executable(${PROJECT_NAME} ${EXAMPLE_SOURCES})

if (BUILD_EDITOR)
    target_link_libraries(${PROJECT_NAME} PRIVATE ${STELLA_LIBRARY_NAME} editor::editor)
    target_compile_definitions(${PROJECT_NAME} PRIVATE STELLA_BUILD_EDITOR)
else()
    target_link_libraries(${PROJECT_NAME} PRIVATE ${STELLA_LIBRARY_NAME})
endif()

if (WIN32)
    # Copy assets directory
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${PROJECT_SOURCE_DIR}/assets"
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets)
    
    ## Copy scripts directory
    #add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        #COMMAND ${CMAKE_COMMAND} -E copy_directory
        #"${PROJECT_SOURCE_DIR}/scripts"
        #$<TARGET_FILE_DIR:${PROJECT_NAME}>/scripts)
    
    # Copy DLLS
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/lib/win/dll-x64"
        $<TARGET_FILE_DIR:${PROJECT_NAME}>)

    # Copy config directory
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/config"
        $<TARGET_FILE_DIR:${PROJECT_NAME}>)
else()
    # Copy assets directory
    file(COPY assets DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

    # Copy scripts directory
    file(COPY scripts DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

    # Copy scenes directory
    file(COPY scenes DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

    # Copy config file
    file(COPY config.json DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

    # Copy config directory
    file(COPY ${CMAKE_SOURCE_DIR}/config DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
endif()
