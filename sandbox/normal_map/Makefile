CXX = g++
CXXFLAGS = -Wall -std=c++17

build: main.cpp display.cpp shader.cpp texture.cpp
	$(CXX) $(CXXFLAGS) main.cpp display.cpp shader.cpp texture.cpp -o sandbox -lGL -lSDL2 -ldl ./libglad.a

