project(editor)

add_subdirectory(imgui)

add_library(${PROJECT_NAME} STATIC debug_layer.cpp
                                   editor_gui.cpp
                                   inspector.cpp
                                   map_editor.cpp
                                   tileset_editor.cpp)
add_library(editor::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
target_link_libraries(${PROJECT_NAME} editor::imgui ${STELLA_LIBRARY_NAME})

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)