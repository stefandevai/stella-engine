project(tests)

set(TEST_SOURCES main.cpp
                 stella/graphics.cpp
                 stella/grid.cpp) 
add_executable(${PROJECT_NAME} ${TEST_SOURCES})
target_link_libraries(tests PRIVATE ${STELLA_LIBRARY_NAME})

if (WIN32)
    # Copy assets folder
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${PROJECT_SOURCE_DIR}/stella/assets"
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets)
    
    # Copy DLLS
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/lib/win/dll-x64"
        $<TARGET_FILE_DIR:${PROJECT_NAME}>)
else()
    file(COPY stella/assets DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
endif()
