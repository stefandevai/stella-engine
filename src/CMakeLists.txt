include_directories(../lib)

if (STELLA_BUILD_EDITOR)
  file(GLOB_RECURSE SOURCE ${PROJECT_SOURCE_DIR}/src/*.cpp)
else()
  file(GLOB_RECURSE SOURCE ${PROJECT_SOURCE_DIR}/src/stella/*.cpp)
elseif()
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
add_executable(stella ../stella.cpp ${SOURCE})

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" OR
    "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" OR
      "${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang")
  set(CXX_COMPILE_FLAGS "-std=gnu++11 -Wall")
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
  message(STATUS "Using MSVC")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CXX_COMPILE_FLAGS}")

target_link_libraries(stella ${GLAD_LIBRARIES} ${SDL2_LIBRARIES} ${OPENGL_LIBRARIES}
  ${OpenAL_LIBRARIES} ${ALUT_LIBRARIES} ${VORBIS_LIBRARIES} ${PLATFORM_LIBRARIES})

