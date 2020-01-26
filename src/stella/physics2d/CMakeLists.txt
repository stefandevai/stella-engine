project(physics2d)

add_library(${PROJECT_NAME} STATIC body.cpp
                                   grid_world.cpp
                                   water_surface.cpp
                                   world.cpp)
add_library(${STELLA_LIBRARY_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/stella/${PROJECT_NAME})