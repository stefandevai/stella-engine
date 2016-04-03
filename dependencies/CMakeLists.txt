include_directories(.)

if (NOT USE_SHARED_GLFW)
  add_subdirectory(glfw)
endif()

if (NOT USE_SHARED_GLEW)
  add_subdirectory(glew)
endif()

if (NOT USE_SHARED_SOIL)
  add_subdirectory(soil)
endif()

if (NOT USE_SHARED_OPENAL)
  add_subdirectory(openal-soft)
  add_subdirectory(freealut)
endif()

if (NOT USE_SHARED_VORBIS)
  add_subdirectory(ogg)
  add_subdirectory(vorbis)
endif()

