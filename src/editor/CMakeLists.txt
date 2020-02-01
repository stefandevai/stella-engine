project(editor)

add_library(${PROJECT_NAME} STATIC debug_layer.cpp
                                   editor_gui.cpp
                                   inspector.cpp
                                   map_editor.cpp
                                   tileset_editor.cpp
                                   scene.cpp
                                   toolbar.cpp
                                   ${CMAKE_SOURCE_DIR}/lib/imgui/examples/imgui_impl_sdl.cpp
                                   ${CMAKE_SOURCE_DIR}/lib/imgui/examples/imgui_impl_opengl3.cpp
                                   ${CMAKE_SOURCE_DIR}/lib/imgui/imgui.cpp
                                   ${CMAKE_SOURCE_DIR}/lib/imgui/imgui_widgets.cpp
                                   ${CMAKE_SOURCE_DIR}/lib/imgui/imgui_draw.cpp)
add_library(editor::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
target_link_libraries(${PROJECT_NAME} ${STELLA_LIBRARY_NAME})

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
                                                  ${CMAKE_SOURCE_DIR}/lib/imgui)