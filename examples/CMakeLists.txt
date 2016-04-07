# Build Examples

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/examples)

macro (build_example EXAMPLE_NAME EXAMPLE_SRC_DIR)
  file(GLOB_RECURSE EXAMPLE_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/${EXAMPLE_SRC_DIR}/*.cpp)
  add_executable(${EXAMPLE_NAME} ${EXAMPLE_SOURCES})
  target_link_libraries(${EXAMPLE_NAME} ${STELLA_LIBRARY_NAME})
endmacro()

build_example("example" "early_example")

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/assets DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

