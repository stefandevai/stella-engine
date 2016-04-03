cmake_minimum_required(VERSION 3.0)
project(stella C CXX)

# Turn on these options to use the libraries contained in your machine.
option(USE_SHARED_GLFW "Use shared GLFW library" off)
option(USE_SHARED_GLEW "Use shared GLEW library" off)
option(USE_SHARED_SOIL "Use shared SOIL library" off)
option(USE_SHARED_OPENAL "Use shared OpenAL and ALut libraries" off)
option(USE_SHARED_VORBIS "Use shared Vorbis and ogg libraries" off)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/modules)
set(DEPENDENCIES_DIR ${PROJECT_SOURCE_DIR}/dependencies)

find_package(OpenGL REQUIRED)

# GLFW - Windowing and input library for OpenGL.
if (USE_SHARED_GLFW)
  find_package(PkgConfig REQUIRED)
  pkg_search_module(GLFW REQUIRED glfw3)
else()
  set(GLFW_INCLUDE_DIRS ${DEPENDENCIES_DIR}/glfw/include)
  set(GLFW_LIBRARIES glfw)
endif()
include_directories(${GLFW_INCLUDE_DIRS})

# GLEW - Library for querying and loading OpenGL extensions.
if (USE_SHARED_GLEW)
  find_package(GLEW REQUIRED)
else()
  add_definitions(-DGLEW_STATIC)
  set(GLEW_INCLUDE_DIRS ${DEPENDENCIES_DIR}/glew/include)
  set(GLEW_LIBRARIES libglew_static)
endif()
include_directories(${GLEW_INCLUDE_DIRS})

# SOIL - Handles image loading in various formats.
if (USE_SHARED_SOIL)
  find_package(SOIL REQUIRED)
else()
  set(SOIL_INCLUDE_DIRS ${DEPENDENCIES_DIR}/soil/src)
  set(SOIL_LIBRARIES SOIL)
endif()
include_directories(${SOIL_INCLUDE_DIRS})

# OpenAL - Audio library (Using OpenAL-Soft for static library).
# ALut - Utility Toolkit for OpenAL.
if (USE_SHARED_OPENAL)
  find_package(OpenAL REQUIRED)
  find_package(ALUT REQUIRED)
else()
  set(OpenAL_INCLUDE_DIRS ${DEPENDENCIES_DIR}/openal-soft/include)
  if (WIN32)
    set(OpenAL_LIBRARIES OpenAL32)
  else()
    set(OpenAL_LIBRARIES openal)
  endif()
  
  set(ALUT_INCLUDE_DIRS ${DEPENDENCIES_DIR}/freealut/include)
  set(ALUT_LIBRARIES alut)
endif()
include_directories(${OpenAL_INCLUDE_DIRS})
include_directories(${ALUT_INCLUDE_DIRS})

# Vorbis - Library for handling .ogg audios.
if (USE_SHARED_VORBIS)
  find_package(Ogg REQUIRED)
  find_package(Vorbis REQUIRED)
else()
  set(OGG_INCLUDE_DIRS ${DEPENDENCIES_DIR}/ogg/include)
  set(OGG_LIBRARIES ogg)

  set(VORBIS_INCLUDE_DIR ${DEPENDENCIES_DIR}/vorbis/include)
  set(VORBIS_LIBRARIES vorbisfile vorbis)
endif()
include_directories(${OGG_INCLUDE_DIRS})
include_directories(${VORBIS_INCLUDE_DIR})

add_subdirectory(src) # Stella source code
add_subdirectory(${DEPENDENCIES_DIR}) # Dependencies folder

