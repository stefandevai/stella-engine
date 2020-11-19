include_directories(.)

if (NOT USE_SYSTEM_SDL2 AND NOT WIN32)
  add_subdirectory(sdl2)
endif()

if (NOT BUILD_EMSCRIPTEN)
  add_subdirectory(glad)
endif()

if (STELLA_DEBUG_MODE)
  include_directories(imgui)
endif()

option(SPDLOG_BUILD_SHARED "" ON)
add_subdirectory(spdlog)

