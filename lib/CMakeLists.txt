include_directories(.)

if (NOT USE_SYSTEM_SDL2 AND NOT WIN32)
  add_subdirectory(sdl2)
endif()

add_subdirectory(glad)

include_directories(imgui)

add_subdirectory(spdlog)

add_subdirectory(json)

add_subdirectory(tinyobjloader)
