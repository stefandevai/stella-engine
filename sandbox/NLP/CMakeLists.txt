cmake_minimum_required(VERSION 3.8...3.16)
project(nlp)


add_executable(${PROJECT_NAME} main.cpp
                               lemmatizer.cpp
                               sentence_tokenizer.cpp
                               word_tokenizer.cpp)
target_link_libraries(${PROJECT_NAME} ${CMAKE_SOURCE_DIR}/lib/libmeta-greedy-tagger.a
                                      ${CMAKE_SOURCE_DIR}/lib/libmeta-crf.a
                                      ${CMAKE_SOURCE_DIR}/lib/libmeta-sequence.a
                                      ${CMAKE_SOURCE_DIR}/lib/libmeta-io.a
                                      ${CMAKE_SOURCE_DIR}/lib/libmeta-util.a
                                      ${CMAKE_SOURCE_DIR}/lib/libmeta-utf.so
                                      -pthread
                                      -lz
                                      -lstdc++fs)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
target_include_directories(${PROJECT_NAME} PRIVATE include/)