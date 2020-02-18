cmake_minimum_required(VERSION 3.8...3.16)

if(CMP0048 EQUAL OLD)
	cmake_policy(SET CMP0048 NEW)
endif()

project(StellaEngine VERSION 0.4
					 DESCRIPTION "OpenGL 2D game engine"
					 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED on)
set(CMAKE_CXX_EXTENSIONS off)

# Build emscripten version
option(BUILD_EMSCRIPTEN "Build emscripten" off)
if (BUILD_EMSCRIPTEN)
  set(CMAKE_C_COMPILER /usr/local/bin/emcc)
  set(CMAKE_CXX_COMPILER /usr/local/bin/em++)
endif()

if (UNIX AND NOT APPLE)
	set(OpenGL_GL_PREFERENCE "GLVND")
  	set (LINUX TRUE)
endif()

if (APPLE)
  set(CMAKE_MACOSX_RPATH 1)
endif()

# Library name
set(STELLA_LIBRARY_NAME "stella")

# Debug mode
option(STELLA_DEBUG_MODE "Build in debug mode" off)

# Build editor
option(STELLA_BUILD_EDITOR "Build engine editor" off)

# Turn this option on to build a targeted example
option(BUILD_STELLA_EXAMPLE "Build example" on)

# Build tests
option(BUILD_STELLA_TESTS "Build tests" off)

# Generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS off)

# Turn on these options to use the libraries contained in your machine (Linux and OSX only).
option(USE_SYSTEM_SDL2 "Search your system for SDL2 library" on)
option(USE_SYSTEM_OPENAL "Search your system for OpenAL and ALut libraries" on)
option(USE_SYSTEM_VORBIS "Search your system for ogg and vorbis libraries" on)
option(USE_SYSTEM_ENTT "Search your system for entt library" off)
option(USE_SYSTEM_FREETYPE "Search your system for FreeType2 library" on)
option(USE_SYSTEM_LUA "Search your system for lua" on)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules)
set(DEPENDENCIES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(SYSTEM)

find_package(OpenGL REQUIRED)

# SDL2 - Windowing and input library for OpenGL.
if (USE_SYSTEM_SDL2 AND NOT WIN32)
    #find_package(PkgConfig REQUIRED)
    #pkg_search_module(SDL2 REQUIRED sdl2)
    find_package(SDL2 REQUIRED)
elseif(WIN32)
    if (MSVC)
      set(SDL2_INCLUDE_DIRS ${DEPENDENCIES_DIR}/sdl2/include ${DEPENDENCIES_DIR}/sdl2/include/SDL2)
      set(SDL2_LIBRARIES ${DEPENDENCIES_DIR}/win/sdl2-x64/lib/x64/SDL2.lib)
    endif()
else()
    set(SDL2_INCLUDE_DIRS ${DEPENDENCIES_DIR}/sdl2/include ${DEPENDENCIES_DIR}/sdl2/include/SDL2)
  	set(SDL2_LIBRARIES SDL2)
endif()
include_directories(${SDL2_INCLUDE_DIRS})

# Dear ImGui
if (STELLA_BUILD_EDITOR)
  add_compile_definitions(STELLA_BUILD_EDITOR)
  include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/editor/imgui)
endif()

# GLAD
if (NOT BUILD_EMSCRIPTEN)
  set(GLAD_INCLUDE_DIRS ${DEPENDENCIES_DIR}/glad/include)
  set(GLAD_LIBRARIES glad)
  include_directories(${GLAD_INCLUDE_DIRS})
endif()

# GLM - OpenGL maths library
include_directories(${DEPENDENCIES_DIR}/glm)

# stb_image - Handles image loading in various formats.
include_directories(${DEPENDENCIES_DIR}/stb_image)

# OpenAL - Audio library (Using OpenAL-Soft for static library).
# ALut - Utility Toolkit for OpenAL.
if (USE_SYSTEM_OPENAL AND NOT WIN32)
	find_package(OpenAL REQUIRED)
	find_package(ALUT REQUIRED)
elseif (WIN32)
	if (MINGW)
		add_definitions(-D_TIMESPEC_DEFINED)
		set(OpenAL_INCLUDE_DIRS ${DEPENDENCIES_DIR}/openal-soft/include)
		set(OpenAL_LIBRARIES OpenAL32)
	elseif(MSVC)
		set(OpenAL_INCLUDE_DIRS ${DEPENDENCIES_DIR}/openal-soft/include)
		set(OpenAL_LIBRARIES ${DEPENDENCIES_DIR}/win/openal-soft-x64/OpenAL32.lib)

		set(ALUT_LIBRARIES ${DEPENDENCIES_DIR}/win/freealut-x64/alut.lib
						   ${DEPENDENCIES_DIR}/win/freealut-x64/alut_static.lib)
		set(ALUT_INCLUDE_DIRS ${DEPENDENCIES_DIR}/freealut/include)
	endif()
else()
	if (APPLE) # Apple already comes with OpenAL
		find_library(OPENAL_LIBRARY OpenAL)
		set(OpenAL_LIBRARIES ${OPENAL_LIBRARY})
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
if (USE_SYSTEM_VORBIS AND NOT MSVC)
	find_package(Ogg REQUIRED)
	find_package(Vorbis REQUIRED)
elseif(MSVC)
	set(OGG_INCLUDE_DIRS ${DEPENDENCIES_DIR}/ogg/include)
	set(OGG_LIBRARIES ${DEPENDENCIES_DIR}/win/ogg-x64/libogg.lib)

	set(VORBIS_INCLUDE_DIR ${DEPENDENCIES_DIR}/vorbis/include)
	set(VORBIS_LIBRARIES ${DEPENDENCIES_DIR}/win/vorbis-x64/vorbis.lib
						 ${DEPENDENCIES_DIR}/win/vorbis-x64/vorbisfile.lib
						 ${DEPENDENCIES_DIR}/win/vorbis-x64/vorbisenc.lib)
endif()
include_directories(${OGG_INCLUDE_DIRS})
include_directories(${VORBIS_INCLUDE_DIR})

# Entt - Entity Component System
if (USE_SYSTEM_ENTT)
	find_package(entt)
else()
	set(ENTT_INCLUDE_DIR ${DEPENDENCIES_DIR}/entt/src)
	#set(ENTT_LIBRARIES entt)
endif()
include_directories(${ENTT_INCLUDE_DIR})

if (USE_SYSTEM_FREETYPE AND NOT MSVC)
	find_package(Freetype)
else()
	set(FREETYPE_INCLUDE_DIRS ${DEPENDENCIES_DIR}/win/freetype-x64/include)
	set(FREETYPE_LIBRARIES ${DEPENDENCIES_DIR}/win/freetype-x64/win64/freetype.lib)
endif()
include_directories(${FREETYPE_INCLUDE_DIRS})

# Lua - Lua scripting language
if(USE_SYSTEM_LUA AND NOT MSVC)
	find_package(Lua REQUIRED)
else()
	if(APPLE)
		set(LUA_LIBRARIES ${DEPENDENCIES_DIR}/lua/macos/liblua53.a)
		set(LUA_INCLUDE_DIR ${DEPENDENCIES_DIR}/lua/macos)
	elseif(LINUX)
		set(LUA_LIBRARIES ${DEPENDENCIES_DIR}/lua/linux/liblua.a)
		set(LUA_INCLUDE_DIR ${DEPENDENCIES_DIR}/lua/linux)
	elseif(MSVC)
		set(LUA_LIBRARIES ${DEPENDENCIES_DIR}/lua/win64/lua53.lib)
		set(LUA_INCLUDE_DIR ${DEPENDENCIES_DIR}/lua/win64/include)
	endif()
endif()

include_directories(${LUA_INCLUDE_DIR})

# Sol3 - Lua API wrapper
set(SOL_INCLUDE_DIR ${DEPENDENCIES_DIR}/sol2/include)
include_directories(${SOL_INCLUDE_DIR})

# Cereal - Serialization library
set(CEREAL_INCLUDE_DIR ${DEPENDENCIES_DIR}/cereal/include)
include_directories(${CEREAL_INCLUDE_DIR})

# MetaToolkit - NLP Library
if (LINUX)
    set(META_TOOLKIT_LIBRARIES ${CMAKE_SOURCE_DIR}/lib/meta_toolkit/linux/libmeta-greedy-tagger.a
                       ${CMAKE_SOURCE_DIR}/lib/meta_toolkit/linux/libmeta-sequence.a
                       ${CMAKE_SOURCE_DIR}/lib/meta_toolkit/linux/libmeta-io.a
                       ${CMAKE_SOURCE_DIR}/lib/meta_toolkit/linux/libmeta-util.a
                       ${CMAKE_SOURCE_DIR}/lib/meta_toolkit/linux/libmeta-utf.so
                       -pthread
                       -lz
                       -lstdc++fs)
endif()

set(META_TOOLKIT_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/lib/meta_toolkit/include)
include_directories(${META_TOOLKIT_INCLUDE_DIR})

# Platform specific libraries
set(PLATFORM_LIBRARIES)

if (LINUX) 
  	if (NOT USE_SYSTEM_SDL2)
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
	find_library(CARBon_LIBRARY Carbon)

	list(APPEND PLATFORM_LIBRARIES ${COCOA_LIBRARY} ${IOKIT_LIBRARY} ${COREVIDEO_LIBRARY} ${CARBon_LIBRARY})
endif()

add_subdirectory(${DEPENDENCIES_DIR}) # Dependencies folder

if (UNIX)
	if(STELLA_DEBUG_MODE)
		#set(CXX_COMPILE_FLAGS "-std=gnu++1z -Wall -g -O0")
    set(CXX_COMPILE_FLAGS "-std=c++17 -Wall -g -O0")

    	# Uncomment for profiling
		#set(CXX_COMPILE_FLAGS "-std=gnu++11 -Wall -pg -g -O0 -no-pie")
		#set(CXX_LINK_FLAGS "-pg")
	else()
		set(CXX_COMPILE_FLAGS "-std=c++17 -O2")
	endif()
elseif(WIN32)
	set(CXX_COMPILE_FLAGS "/wd4068")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CXX_COMPILE_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${CXX_LINK_FLAGS}")

add_subdirectory(src)

# Build examples if specified
if (BUILD_STELLA_EXAMPLE)
  add_subdirectory(nikte)
endif()

# Build tests if specified
if (BUILD_STELLA_TESTS)
  add_subdirectory(test)
endif()