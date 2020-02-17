project(nlp)

add_library(${PROJECT_NAME} STATIC nlp.cpp
                                   lemmatizer.cpp
                                   sentence_tokenizer.cpp
                                   word_tokenizer.cpp)

add_library(${STELLA_LIBRARY_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

# if (LINUX)
#     set(META_TOOLKIT_LIBRARIES ${CMAKE_SOURCE_DIR}/lib/meta_toolkit/linux/libmeta-greedy-tagger.a
#                        ${CMAKE_SOURCE_DIR}/lib/meta_toolkit/linux/libmeta-sequence.a
#                        ${CMAKE_SOURCE_DIR}/lib/meta_toolkit/linux/libmeta-io.a
#                        ${CMAKE_SOURCE_DIR}/lib/meta_toolkit/linux/libmeta-util.a
#                        ${CMAKE_SOURCE_DIR}/lib/meta_toolkit/linux/libmeta-utf.so
#                        -pthread
#                        -lz
#                        -lstdc++fs)
# endif()

# set(META_TOOLKIT_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/lib/meta_toolkit/include)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/stella/${PROJECT_NAME})

# target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/stella/${PROJECT_NAME}
                                                #   ${META_TOOLKIT_INCLUDE_DIR})