project(graphics)

add_library(${PROJECT_NAME} STATIC animator.cpp
                                   display.cpp
                                   font.cpp
                                   framebuffer.cpp
                                   renderer.cpp
                                   renderable.cpp
                                   sprite_renderer.cpp
                                   shader.cpp
                                   shape.cpp
                                   shape_renderer.cpp
                                   texture.cpp
                                   layers/layer.cpp
                                   layers/sprite_layer.cpp
                                   layers/shape_layer.cpp)
                                   
add_library(${STELLA_LIBRARY_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

if (STELLA_BUILD_EDITOR)
    target_link_libraries(${PROJECT_NAME} PRIVATE editor::editor)
endif()

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/stella/${PROJECT_NAME})