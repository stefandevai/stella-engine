project(${STELLA_LIBRARY_NAME})

add_subdirectory(audio)
add_subdirectory(core)
add_subdirectory(graphics)
add_subdirectory(nlp)
add_subdirectory(npc)
add_subdirectory(physics2d)
add_subdirectory(systems)
add_subdirectory(topdown)

add_library(${PROJECT_NAME} STATIC application.cpp
                                   game.cpp)
add_library(${STELLA_LIBRARY_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
target_link_libraries(${PROJECT_NAME} PUBLIC 
                                      ${STELLA_LIBRARY_NAME}::systems
                                      ${STELLA_LIBRARY_NAME}::audio
                                      ${STELLA_LIBRARY_NAME}::core
                                      ${STELLA_LIBRARY_NAME}::graphics
                                      ${STELLA_LIBRARY_NAME}::nlp
                                      ${STELLA_LIBRARY_NAME}::npc
                                      ${STELLA_LIBRARY_NAME}::physics2d
                                      ${STELLA_LIBRARY_NAME}::topdown
                                      OpenGL::GL
                                      ${GLAD_LIBRARIES}
                                      ${SDL2_LIBRARIES}
                                      ${ENTT_LIBRARIES}
                                      ${FREETYPE_LIBRARIES}
                                      ${LUA_LIBRARIES}
                                      ${PLATFORM_LIBRARIES})
                                      
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/stella/ ${OPENGL_INCLUDE_DIR})

#if (UNIX AND STELLA_DEBUG_MODE)
  #target_compile_options(${PROJECT_NAME} PRIVATE "-Wold-style-cast")
#endif()

#if(MSVC)
  #target_compile_options(${PROJECT_NAME} PRIVATE "-wd4068")
#endif()
