project(graphics)

add_library(${PROJECT_NAME} STATIC animator.cpp
                                   display.cpp
                                   font.cpp
                                   framebuffer.cpp
                                   renderer2.cpp
                                   renderable.cpp
                                   sprite_renderer2.cpp
                                   shader.cpp
                                   shape.cpp
                                   shape_renderer2.cpp
                                   texture.cpp
                                   layers/layer2.cpp
                                   layers/sprite_layer2.cpp
                                   layers/shape_layer2.cpp)
                                   
add_library(${STELLA_LIBRARY_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

if (STELLA_BUILD_EDITOR)
    target_link_libraries(${PROJECT_NAME} PRIVATE editor::editor)
endif()

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/stella/${PROJECT_NAME})