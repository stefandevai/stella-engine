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
                                   resource/texture_loader.cpp)
                                   
add_library(${STELLA_LIBRARY_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME} PRIVATE ${STELLA_LIBRARY_NAME}::components)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/stella/${PROJECT_NAME})
