include_directories(.)

if (NOT USE_SHARED_SDL2)
  add_subdirectory(sdl2)
endif()

if (NOT BUILD_EMSCRIPTEN)
  add_subdirectory(glad)
endif()

if (NOT USE_SHARED_OPENAL)
  if (NOT APPLE) # Apple already comes with OpenAL
    add_subdirectory(openal-soft)
  endif()
  add_subdirectory(freealut)
endif()

if (NOT USE_SHARED_VORBIS)
  add_subdirectory(ogg)
  add_subdirectory(vorbis)
endif()

