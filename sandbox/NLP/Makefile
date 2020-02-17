CXX = g++
CXXFLAGS = -Wall -std=c++17
INCLUDE = -I./include
LIB = -L./lib/libmeta-index.a -L./lib/libmeta-classify.a -L./lib/libmeta-regression.a -L./lib/libmeta-stats.a -L./lib/libmeta-parser.a -L./lib/libmeta-features.a -L./lib/libmeta-language-model.a -L./lib/libmeta-succint.a -L./lib/libmeta-topics.a

build: main.cpp sentence_tokenizer.cpp word_tokenizer.cpp lemmatizer.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE) main.cpp sentence_tokenizer.cpp word_tokenizer.cpp lemmatizer.cpp -o nlp $(LIB) 

