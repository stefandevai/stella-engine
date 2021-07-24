project(systems)

add_library(${PROJECT_NAME} STATIC animation_player.cpp
                                   # character_animation.cpp
                                   render2.cpp
                                   group.cpp
                                   # npc.cpp
                                   physics.cpp
                                   # render.cpp
                                   # speech.cpp
                                   text.cpp
                                   # tile.cpp
                                   # timer.cpp
                                   )

target_link_libraries(${PROJECT_NAME} PRIVATE ${STELLA_LIBRARY_NAME}::graphics)

add_library(${STELLA_LIBRARY_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/stella/${PROJECT_NAME})
