include_directories(.)

if (NOT USE_SYSTEM_SDL2 AND NOT WIN32)
  add_subdirectory(sdl2)
endif()

if (NOT BUILD_EMSCRIPTEN)
  add_subdirectory(glad)
endif()

if (NOT USE_SYSTEM_OPENAL AND NOT WIN32)
  if (NOT APPLE) # Apple already comes with OpenAL
    add_subdirectory(openal-soft)
  endif()
  add_subdirectory(freealut)
endif()

if (NOT USE_SYSTEM_VORBIS AND NOT WIN32)
  add_subdirectory(ogg)
  add_subdirectory(vorbis)
endif()

if (STELLA_DEBUG_MODE)
  include_directories(imgui)
  #add_subdirectory(imgui)
endif()

