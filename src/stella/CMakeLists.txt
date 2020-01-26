project(${STELLA_LIBRARY_NAME})

add_subdirectory(audio)
add_subdirectory(core)
add_subdirectory(graphics)
add_subdirectory(physics2d)
add_subdirectory(topdown)

add_library(${PROJECT_NAME} STATIC application.cpp
                                   game.cpp)
add_library(${STELLA_LIBRARY_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
target_link_libraries(${PROJECT_NAME} PUBLIC 
                                      ${STELLA_LIBRARY_NAME}::audio
                                      ${STELLA_LIBRARY_NAME}::core
                                      ${STELLA_LIBRARY_NAME}::graphics
                                      ${STELLA_LIBRARY_NAME}::physics2d
                                      ${STELLA_LIBRARY_NAME}::topdown)
                                      
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/stella/)