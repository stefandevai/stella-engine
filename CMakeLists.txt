cmake_minimum_required(VERSION 3.0)
project(stella)

file(GLOB_RECURSE SOURCE ${PROJECT_SOURCE_DIR}/src/*.cpp)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules)

find_package(OpenGL REQUIRED)

find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)
include_directories(${GLFW_INCLUDE_DIRS})

find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIRS})

find_package(SOIL REQUIRED)
include_directories(${SOIL_INCLUDE_DIRS})

add_executable(stella game.cpp ${SOURCE})

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" OR
    "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" OR
    "${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang")
  set(CXX_COMPILE_FLAGS "-std=c++11 -Wall")
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
  message(STATUS "Using MSVC")
endif()

if (UNIX AND NOT APPLE)
  set (LINUX TRUE)
endif()

if (LINUX)
  set(CXX_LINK_FLAGS "-lX11 -lpthread -lXrandr -lXi -lGL")
elseif(APPLE)
  set(CXX_LINK_FLAGS "-framework Cocoa -framework OpenGL -framework IOKit -framework CoreVideo -framework Carbon")
  set(GLFW_LIBRARIES "glfw3")
elseif(WIN32)
  set(CXX_LINK_FLAGS "-lopengl32")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CXX_COMPILE_FLAGS}")
target_link_libraries(stella ${CXX_LINK_FLAGS} ${GLEW_LIBRARIES} ${GLFW_LIBRARIES} ${SOIL_LIBRARIES})

