include_directories(../dependencies)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})

file(GLOB_RECURSE SOURCE ${PROJECT_SOURCE_DIR}/src/*.cpp)

add_executable(stella ../game.cpp ${SOURCE})

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" OR
    "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" OR
      "${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang")
  set(CXX_COMPILE_FLAGS "-std=gnu++11 -Wall")
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
  message(STATUS "Using MSVC")
endif()

if (UNIX AND NOT APPLE)
  set (LINUX TRUE)
endif()

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
  set(CXX_LINK_FLAGS "-framework Cocoa -framework OpenGL -framework IOKit -framework CoreVideo -framework Carbon")
  set(GLFW_LIBRARIES "glfw3")
elseif(WIN32)
  set(CXX_LINK_FLAGS "-lopengl32")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CXX_COMPILE_FLAGS}")

target_link_libraries(stella ${GLEW_LIBRARIES} ${GLFW_LIBRARIES} ${OPENGL_LIBRARIES} ${SOIL_LIBRARIES}
  ${OpenAL_LIBRARIES} ${ALUT_LIBRARIES} ${VORBIS_LIBRARIES} ${PLATFORM_LIBRARIES})

