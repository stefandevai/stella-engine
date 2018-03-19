cmake_minimum_required(VERSION 3.0)
project(STELLA C CXX)

if (UNIX AND NOT APPLE)
  set (LINUX TRUE)
endif()
if (APPLE)
  set(CMAKE_MACOSX_RPATH 1)
endif()

# Library name
set(STELLA_LIBRARY_NAME "stella")

# Turn this option on to build a targeted example
option(BUILD_STELLA_EXAMPLE "Build example" on)
if (BUILD_STELLA_EXAMPLE)
	set(EXAMPLE_DIR "worm")
endif()

# Turn on these options to use the libraries contained in your machine.
option(USE_SHARED_GLFW "Use shared GLFW library" on)
option(USE_SHARED_GLEW "Use shared GLEW library" on)
option(USE_SHARED_SOIL "Use shared SOIL library" off)
option(USE_SHARED_OPENAL "Use shared OpenAL and ALut libraries" on)
option(USE_SHARED_VORBIS "Use shared Vorbis and ogg libraries" on)
option(USE_SHARED_ENTITYX "Use shared EntityX library" off)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules)
set(DEPENDENCIES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/dependencies)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

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

# GLM - OpenGL maths library
include_directories(${DEPENDENCIES_DIR}/glm)

# SOIL - Handles image loading in various formats.
if (USE_SHARED_SOIL)
	find_package(SOIL REQUIRED)
else()
	set(SOIL_INCLUDE_DIRS ${DEPENDENCIES_DIR}/soil/code)
	set(SOIL_LIBRARIES soil)
endif()
include_directories(${SOIL_INCLUDE_DIRS})

# OpenAL - Audio library (Using OpenAL-Soft for static library).
# ALut - Utility Toolkit for OpenAL.
if (USE_SHARED_OPENAL)
	find_package(OpenAL REQUIRED)
	if (APPLE)
		set(OpenAL_LIBRARIES ${OPENAL_LIBRARY})
	endif()
	find_package(ALUT REQUIRED)
else()
	if (APPLE) # Apple already comes with OpenAL
		find_library(OPENAL_LIBRARY OpenAL)
		set(OpenAL_LIBRARIES ${OPENAL_LIBRARY})
	elseif (WIN32)
		if (MINGW)
			 add_definitions(-D_TIMESPEC_DEFINED)
		endif()
		set(OpenAL_INCLUDE_DIRS ${DEPENDENCIES_DIR}/openal-soft/include)
		set(OpenAL_LIBRARIES OpenAL32)
	else()
		set(OpenAL_INCLUDE_DIRS ${DEPENDENCIES_DIR}/openal-soft/include)
		set(OpenAL_LIBRARIES openal)
	endif()
	
	set(ALUT_INCLUDE_DIRS ${DEPENDENCIES_DIR}/freealut/include)
	set(ALUT_LIBRARIES alut)
endif()
include_directories(${OpenAL_INCLUDE_DIRS})
include_directories(${ALUT_INCLUDE_DIRS})

# Ogg Vorbis - Libraries for handling .ogg audio format.
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

# EntityX - Library implementing an Entity-Component-System architecture
if (USE_SHARED_ENTITYX)
	find_package(entityx)
else()
	set(ENTITYX_INCLUDE_DIR ${DEPENDENCIES_DIR}/entityx)
	set(ENTITYX_LIBRARIES entityx)
endif()
include_directories(${ENTITYX_INCLUDE_DIR})

# Platform specific libraries
set(PLATFORM_LIBRARIES)

if (LINUX)
	if (NOT USE_SHARED_GLFW)
		find_package(X11 REQUIRED)
		include_directories(${X11_INCLUDE_DIR})
		list(APPEND PLATFORM_LIBRARIES ${X11_X11_LIB})
		list(APPEND PLATFORM_LIBRARIES ${X11_LIBRARIES})

		if (X11_Xi_FOUND)
			list(APPEND PLATFORM_LIBRARIES ${X11_Xi_LIB})
		endif()

		if (X11_Xrandr_FOUND)
			list(APPEND PLATFORM_LIBRARIES ${X11_Xrandr_LIB})
		endif()

		if (X11_xf86vmode_FOUND)
			list(APPEND PLATFORM_LIBRARIES ${X11_Xxf86vm_LIB})
			include_directories(${X11_xf86vmode_INCLUDE_PATH})
		endif()

		if (X11_Xinerama_FOUND)
			list(APPEND PLATFORM_LIBRARIES ${X11_Xinerama_LIB})
		endif()

		if (X11_Xcursor_FOUND)
			list(APPEND PLATFORM_LIBRARIES ${X11_Xcursor_LIB})
		endif()

		find_package(LibRt REQUIRED)
		include_directories(${LIBRT_INCLUDE_DIRS})
		list(APPEND PLATFORM_LIBRARIES ${LIBRT_LIBRARIES})

		find_package(LibM REQUIRED)
		include_directories(${LIBM_INCLUDE_DIRS})
		list(APPEND PLATFORM_LIBRARIES ${LIBM_LIBRARIES})
	
		find_package(LibDL REQUIRED)
		include_directories(${LIBDL_INCLUDE_DIR})
		list(APPEND PLATFORM_LIBRARIES ${LIBDL_LIBRARY})

		find_package(Threads REQUIRED)
		list(APPEND PLATFORM_LIBRARIES ${CMAKE_THREAD_LIBS_INIT})
	endif()
elseif(APPLE)
	find_library(COCOA_LIBRARY Cocoa)
	find_library(IOKIT_LIBRARY IOKit)
	find_library(COREVIDEO_LIBRARY CoreVideo)
	find_library(CARBON_LIBRARY Carbon)

	list(APPEND PLATFORM_LIBRARIES ${COCOA_LIBRARY} ${IOKIT_LIBRARY} ${COREVIDEO_LIBRARY} ${CARBON_LIBRARY})
elseif(WIN32)
	set(CXX_LINK_FLAGS "-lopengl32")
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
endif()

add_subdirectory(${DEPENDENCIES_DIR}) # Dependencies folder

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" OR
		"${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" OR
			"${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang")
	set(CXX_COMPILE_FLAGS "-std=gnu++11 -Wall -g -O0")
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
	message(STATUS "Using MSVC")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CXX_COMPILE_FLAGS}")

# Build main library
file(GLOB_RECURSE STELLA_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)
add_library(${STELLA_LIBRARY_NAME} ${STELLA_SOURCES})

target_link_libraries(${STELLA_LIBRARY_NAME} ${GLEW_LIBRARIES} ${GLFW_LIBRARIES} ${OPENGL_LIBRARIES} ${SOIL_LIBRARIES}
	${OpenAL_LIBRARIES} ${ALUT_LIBRARIES} ${VORBIS_LIBRARIES} ${ENTITYX_LIBRARIES} ${PLATFORM_LIBRARIES})

# Build examples if specified
if (BUILD_STELLA_EXAMPLE)
  add_subdirectory(examples)
endif()

