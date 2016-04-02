cmake_minimum_required(VERSION 3.0)
project(stella)

set(DEPENDENCIES_DIR ${PROJECT_SOURCE_DIR}/Dependencies)

# Turn on these options to use the libraries contained in your machine.
option(USE_SHARED_GLFW "Use shared GLFW library" off)
option(USE_SHARED_GLEW "Use shared GLEW library" on)
option(USE_SHARED_SOIL "Use shared SOIL library" off)
option(USE_SHARED_OPENAL "Use shared OpenAL library" off)
option(USE_SHARED_ALUT "Use shared ALut library" off)
option(USE_SHARED_VORBIS "Use shared Vorbis and ogg libraries" on)

file(GLOB_RECURSE SOURCE ${PROJECT_SOURCE_DIR}/src/*.cpp)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/modules)

find_package(OpenGL REQUIRED)
message(STATUS ${OPENGL_LIBRARIES})

# GLFW - Windowing and input library for OpenGL.
if (USE_SHARED_GLFW)
  find_package(PkgConfig REQUIRED)
  pkg_search_module(GLFW REQUIRED glfw3)
  include_directories(${GLFW_INCLUDE_DIRS})
else()
  add_subdirectory(Dependencies/glfw)
  include_directories(Dependencies/glfw/include)
  set(GLFW_LIBRARY "glfw")
endif()

# GLEW - Library for querying and loading OpenGL extensions.
if (USE_SHARED_GLEW)
  find_package(GLEW REQUIRED)
  include_directories(${GLEW_INCLUDE_DIRS})
else()
  add_subdirectory(${DEPENDENCIES_DIR}/glew/build/cmake)
  include_directories(${DEPENDENCIES_DIR}/glew/src)
endif()

# SOIL - Handles image loading in various formats.
if (USE_SHARED_SOIL)
  find_package(SOIL REQUIRED)
  include_directories(${SOIL_INCLUDE_DIRS})
else()
  add_subdirectory(${DEPENDENCIES_DIR}/SOIL)
  include_directories(${DEPENDENCIES_DIR}/SOIL/src)
  set(SOIL_LIBRARIES SOIL)
endif()

# OpenAL - Audio library.
if (USE_SHARED_OPENAL)
  find_package(OpenAL REQUIRED)
  include_directories(${OpenAL_INCLUDE_DIRS})
else()
  add_subdirectory(${DEPENDENCIES_DIR}/OpenAL-Soft/)
  include_directories(${DEPENDENCIES_DIR}/OpenAL-Soft/include)
endif()

# ALut - Utility Toolkit for OpenAL.
if (USE_SHARED_ALUT)
  find_package(ALUT REQUIRED)
  include_directories(${ALUT_INCLUDE_DIRS})
else()
  add_subdirectory(${DEPENDENCIES_DIR}/FreeALut)
  include_directories(${DEPENDENCIES_DIR}/FreeALut/include)
  set(ALUT_LIBRARIES alut)
endif()

# Vorbis - Library for handling .ogg audios.
if (USE_SHARED_VORBIS)
  find_package(Vorbis REQUIRED)
  include_directories(${VORBIS_INCLUDE_DIRS})
else()
  add_subdirectory(${DEPENDENCIES_DIR}/ogg)
  include_directories(${DEPENDENCIES_DIR}/ogg/include)

  add_subdirectory(${DEPENDENCIES_DIR}/Vorbis)
  include_directories(${DEPENDENCIES_DIR}/Vorbis/include)
  #set(VORBIS_LIBRARIES "vorbis")
endif()
message(STATUS ${VORBIS_LIBRARIES})
message(STATUS ${OGG_LIBRARIES})

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
  message(STATUS "Linux")
elseif(APPLE)
  set(CXX_LINK_FLAGS "-framework Cocoa -framework OpenGL -framework IOKit -framework CoreVideo -framework Carbon")
  set(GLFW_LIBRARIES "glfw3")
elseif(WIN32)
  set(CXX_LINK_FLAGS "-lopengl32")
endif()

message(STATUS ${ALUT_LIBRARIES})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CXX_COMPILE_FLAGS}")
target_link_libraries(stella ${CXX_LINK_FLAGS} ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES} ${GLFW_LIBRARY} ${GLFW_LIBRARIES} ${SOIL_LIBRARIES}
  ${OpenAL_LIBRARIES} ${ALUT_LIBRARIES} ${OGG_LIBRARIES} ${VORBIS_LIBRARIES})

