project(imgui)

add_library(${PROJECT_NAME} STATIC imgui.cpp
                                   imgui_widgets.cpp
                                   imgui_impl_sdl.cpp
                                   imgui_impl_opengl3.cpp
                                   imgui_draw.cpp)
add_library(editor::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/editor/imgui)