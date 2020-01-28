project(audio)

add_library(${PROJECT_NAME} STATIC ogg_stream.cpp
                                   sound.cpp
                                   soundplayer.cpp)
add_library(${STELLA_LIBRARY_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
target_link_libraries(${PROJECT_NAME} PRIVATE 
                                      ${OpenAL_LIBRARIES}
                                      ${ALUT_LIBRARIES}
                                      ${VORBIS_LIBRARIES}
                                      ${OGG_LIBRARIES})
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/stella/${PROJECT_NAME})