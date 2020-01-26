project(nikte)

set(EXAMPLE_SOURCES main.cpp
                    game.cpp)
add_executable(${PROJECT_NAME} ${EXAMPLE_SOURCES})
target_link_libraries(${PROJECT_NAME} PRIVATE ${STELLA_LIBRARY_NAME})

file(COPY assets DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY scripts DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
