project(editor)

add_compile_definitions(IMGUI_IMPL_OPENGL_LOADER_GLAD)
add_library(${PROJECT_NAME} STATIC editor.cpp
                                   widgets/chat.cpp
                                   widgets/console.cpp
                                   widgets/inspector.cpp
                                   widgets/map_editor.cpp
                                   widgets/tileset_editor.cpp
                                   widgets/scene.cpp
                                   widgets/toolbar.cpp
                                   widgets/file_dialog.cpp
                                   ${CMAKE_SOURCE_DIR}/lib/imgui/examples/imgui_impl_sdl.cpp
                                   ${CMAKE_SOURCE_DIR}/lib/imgui/examples/imgui_impl_opengl3.cpp
                                   ${CMAKE_SOURCE_DIR}/lib/imgui/imgui.cpp
                                   ${CMAKE_SOURCE_DIR}/lib/imgui/imgui_widgets.cpp
                                   ${CMAKE_SOURCE_DIR}/lib/imgui/imgui_draw.cpp)
add_library(editor::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
target_link_libraries(${PROJECT_NAME} ${STELLA_LIBRARY_NAME} OpenGL::GL ${GLAD_LIBRARIES})

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
                                                  ${CMAKE_SOURCE_DIR}/lib/imgui
                                                  ${OPENGL_INCLUDE_DIR})