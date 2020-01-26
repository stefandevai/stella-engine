project(tests)

set(TEST_SOURCES main.cpp
                 stella/graphics.cpp
                 stella/grid.cpp) 
add_executable(${PROJECT_NAME} ${TEST_SOURCES})
target_link_libraries(tests PRIVATE ${STELLA_LIBRARY_NAME})

file(COPY stella/assets DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
