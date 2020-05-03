project(graphics)

add_library(${PROJECT_NAME} STATIC animator.cpp
                                   display.cpp
                                   font.cpp
                                   framebuffer.cpp
                                   #renderer.cpp
                                   renderer2.cpp
                                   renderable.cpp
                                   #sprite_renderer.cpp
                                   sprite_renderer2.cpp
                                   #sprite_fog_renderer.cpp
                                   shader.cpp
                                   shape.cpp
                                   #shape_renderer.cpp
                                   shape_renderer2.cpp
                                   #sprite.cpp
                                   #spritesheet.cpp
                                   texture.cpp
                                   #layers/fog_layer.cpp
                                   #layers/layer.cpp
                                   layers/layer2.cpp
                                   #layers/basic_layer.cpp
                                   #layers/firelayer.cpp
                                   #layers/sprite_layer.cpp
                                   layers/sprite_layer2.cpp
                                   #layers/shape_layer.cpp
                                   layers/shape_layer2.cpp)
                                   
add_library(${STELLA_LIBRARY_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

if (STELLA_BUILD_EDITOR)
    target_link_libraries(${PROJECT_NAME} PRIVATE editor::editor)
endif()

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/stella/${PROJECT_NAME})