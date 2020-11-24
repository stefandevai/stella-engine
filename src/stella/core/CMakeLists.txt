project(core)

add_library(${PROJECT_NAME} STATIC tile_map.cpp
                                   ecs_lua_api.cpp
                                   asset_manager.cpp
                                   model_loader.cpp
                                   shader_loader.cpp
                                   json.cpp
                                   map_grid.cpp
                                   scene.cpp)
                                   
add_library(${STELLA_LIBRARY_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME} PRIVATE ${STELLA_LIBRARY_NAME}::components)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/stella/${PROJECT_NAME})
