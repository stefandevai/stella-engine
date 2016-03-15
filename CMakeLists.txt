cmake_minimum_required(VERSION 3.0)
project(stella)

file(GLOB_RECURSE SOURCE ${PROJECT_SOURCE_DIR}/src/*.cpp)

#add_subdirectory(glm)

find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)
include_directories(${GLFW_INCLUDE_DIRS})

find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIRS})

set(CXX_COMPILE_FLAGS "-std=c++11 -Wall")
set(CXX_LINK_FLAGS "-lX11 -lpthread -lXrandr -lXi -lGL")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CXX_COMPILE_FLAGS}")

add_executable(stella game.cpp ${SOURCE})

target_link_libraries(stella ${CXX_LINK_FLAGS} ${GLEW_LIBRARIES} ${GLFW_LIBRARIES})

