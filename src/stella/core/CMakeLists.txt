project(core)

add_library(${PROJECT_NAME} STATIC # tile_map.cpp
                                   # ecs_lua_api.cpp
                                   script_api.cpp
                                   json.cpp
                                   # map_grid.cpp
                                   scene.cpp
                                   input/sdl_input_mapper.cpp
                                   resource/asset_manager.cpp
                                   resource/model_loader.cpp
                                   resource/shader_loader.cpp
                                   resource/texture_loader.cpp
                                   resource/font_loader.cpp)
                                   
add_library(${STELLA_LIBRARY_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME} PRIVATE ${STELLA_LIBRARY_NAME}::components nlohmann_json::nlohmann_json tinyobjloader)

# message("HEREEEEEEEEEEEEEEEE")
# message(STATUS "${TINYOBJLOADER_INCLUDE_DIR}")
# message(STATUS "${NLOHMANN_JSON_TARGET_NAME}")
# message(STATUS "${NLOHMANN_JSON_INCLUDE_INSTALL_DIR}")
# message("HEREEEEEEEEEEEEEEEE")
# message("HEREEEEEEEEEEEEEEEE")
# message("HEREEEEEEEEEEEEEEEE")
# message("HEREEEEEEEEEEEEEEEE")
# message("HEREEEEEEEEEEEEEEEE")
# message("HEREEEEEEEEEEEEEEEE")
# message("HEREEEEEEEEEEEEEEEE")

target_include_directories(
  ${PROJECT_NAME}
  PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/include/stella/${PROJECT_NAME}
  # ${CMAKE_CURRENT_SOURCE_DIR}/lib/json/include
  # ${CMAKE_CURRENT_SOURCE_DIR}/lib/tinyobjloader
)
