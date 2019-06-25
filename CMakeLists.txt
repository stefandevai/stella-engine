cmake_minimum_required(VERSION 3.0)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project(STELLA C CXX)

if (UNIX AND NOT APPLE)
  set (LINUX TRUE)
endif()
if (APPLE)
  set(CMAKE_MACOSX_RPATH 1)
endif()

# Library name
set(STELLA_LIBRARY_NAME "stella")

# Debug mode
option(STELLA_DEBUG_MODE "Build in debug mode" OFF)

# Turn this option on to build a targeted example
option(BUILD_STELLA_EXAMPLE "Build example" ON)
if (BUILD_STELLA_EXAMPLE)
	set(EXAMPLE_DIR "torch")
endif()

# Build tests
option(BUILD_STELLA_TESTS "Build tests" OFF)

# Generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Turn on these options to use the libraries contained in your machine.
option(USE_SHARED_SDL2 "Use shared SDL2 library" ON)
option(USE_SHARED_SOIL "Use shared SOIL library" OFF)
option(USE_SHARED_OPENAL "Use shared OpenAL and ALut libraries" ON)
option(USE_SHARED_VORBIS "Use shared Vorbis and ogg libraries" ON)
option(USE_SHARED_ENTITYX "Use shared EntityX library" OFF)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules)
set(DEPENDENCIES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

find_package(OpenGL REQUIRED)

# SDL2 - Windowing and input library for OpenGL.
if (USE_SHARED_SDL2)
  find_package(PkgConfig REQUIRED)
  pkg_search_module(SDL2 REQUIRED sdl2)
else()
  set(SDL2_INCLUDE_DIRS ${DEPENDENCIES_DIR}/sdl2/include)
  set(SDL2_LIBRARIES SDL2)
endif()
include_directories(${SDL2_INCLUDE_DIRS})

# GLAD
set(GLAD_INCLUDE_DIRS ${DEPENDENCIES_DIR}/glad/include)
set(GLAD_LIBRARIES glad)
include_directories(${GLAD_INCLUDE_DIRS})

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
  set(ENTITYX_BUILD_TESTING false CACHE BOOL "Enable building of tests.")
endif()
include_directories(${ENTITYX_INCLUDE_DIR})

# Lua - Lua scripting language
set(LUA_INCLUDE_DIR ${DEPENDENCIES_DIR}/lua)
if(APPLE)
set(LUA_LIBRARIES ${DEPENDENCIES_DIR}/lua/liblua53.a)
endif()
include_directories(${LUA_INCLUDE_DIR})

# LuaBridge - Library for binding Lua to C++
#set(LUABRIDGE_INCLUDE_DIR ${DEPENDENCIES_DIR}/LuaBridge/Source)
#include_directories(${LUABRIDGE_INCLUDE_DIR})

# Sol3 - Lua API wrapper
set(SOL_INCLUDE_DIR ${DEPENDENCIES_DIR}/sol2)
include_directories(${SOL_INCLUDE_DIR})

# Platform specific libraries
set(PLATFORM_LIBRARIES)

if (LINUX)
  if (NOT USE_SHARED_SDL2)
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
    list(APPEND PLATFORM_LIBRARIES ${LIBDL_LIBRARIES})

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
	if (STELLA_DEBUG_MODE)
    set(CXX_COMPILE_FLAGS "-std=gnu++1z -Wall -g -O0")

    # Uncomment for profiling
		#set(CXX_COMPILE_FLAGS "-std=gnu++11 -Wall -pg -g -O0 -no-pie")
		#set(CXX_LINK_FLAGS "-pg")
	else()
		set(CXX_COMPILE_FLAGS "-std=c++1z -O2")
	endif()
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
	message(STATUS "Using MSVC")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CXX_COMPILE_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${CXX_LINK_FLAGS}")

# Build main library
file(GLOB_RECURSE STELLA_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)
add_library(${STELLA_LIBRARY_NAME} ${STELLA_SOURCES})

target_link_libraries(${STELLA_LIBRARY_NAME} ${GLAD_LIBRARIES} ${SDL2_LIBRARIES} ${OPENGL_LIBRARIES} ${SOIL_LIBRARIES}
  ${OpenAL_LIBRARIES} ${ALUT_LIBRARIES} ${VORBIS_LIBRARIES} ${ENTITYX_LIBRARIES} ${LUA_LIBRARIES} ${PLATFORM_LIBRARIES})

# Build examples if specified
if (BUILD_STELLA_EXAMPLE)
  add_subdirectory(example)
endif()

# Build tests if specified
if (BUILD_STELLA_TESTS)
  add_subdirectory(test)
endif()

