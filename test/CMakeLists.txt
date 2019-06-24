set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/test)

macro (build_tests)
  file(GLOB_RECURSE TESTS_SOURCES ${PROJECT_SOURCE_DIR}/test/stella/*.cpp)
  add_executable(tests ${PROJECT_SOURCE_DIR}/test/main.cpp ${TESTS_SOURCES})
  target_link_libraries(tests ${STELLA_LIBRARY_NAME})
endmacro()

file(COPY ${PROJECT_SOURCE_DIR}/test/stella/assets DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

build_tests()

