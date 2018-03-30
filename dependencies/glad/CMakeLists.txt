project(glad C)
cmake_minimum_required(VERSION 2.8)

find_package(OpenGL REQUIRED)
include_directories(include ${OPENGL_INCLUDE_DIR})

find_package(LibDL REQUIRED)
include_directories(include ${LIBDL_INCLUDE_DIR})

add_library(glad src/glad.c)
target_link_libraries(glad ${OPENGL_LIBRARIES} ${LIBDL_LIBRARIES})

