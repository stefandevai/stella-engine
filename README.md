# Stella Engine

![Stella Engine early example](/engine.png)

## Description:

A cross-platform 2D C++/OpenGL game engine. It is currently in its early development phase.

## Current features:

- Spritesheets
- Animations
- Ogg audio streaming

## Building:

The project uses cmake as its build system. It has been tested on Linux and MacOS X, however every dependency is supposed to work on Windows aswell.

### Dependencies

The project already contains all its platform indepent dependencies. They are the following

- [GLFW](https://github.com/glfw/glfw): Library for windowing, handling input and creating an OpenGL context.
- [GLEW](https://github.com/nigels-com/glew): Library for querying and loading OpenGL extensions.
- [SOIL](https://github.com/kbranigan/Simple-OpenGL-Image-Library): Handles image loading in various formats.
- OpenAL: Audio library. *NOTE: Using [OpenAL Soft](https://github.com/kcat/openal-soft) in Linux for building the static library*.
- ALUT: Utility Toolkit for OpenAL. *NOTE: Using [Free ALUT](https://github.com/vancegroup/freealut) for building the static library*.
- [Ogg Vorbis](): Libraries for handling .ogg audio format.
- [EntityX](https://github.com/alecthomas/entityx): Entity-component-system for C++.

However if you want to use the shared libraries in your own computer you can turn on the specific cmake option contained in the top CMakeLists.txt file.
e.g. To use the glfw library contained in your computer open CMakeLists.txt in a text editor and change the following line

```cmake
option(USE_SHARED_GLFW "Use shared GLFW library" off)
```

to

```cmake
option(USE_SHARED_GLFW "Use shared GLFW library" on)
```
### Building example on Linux or MacOS X:

```bash
git clone --recursive https://github.com/stefandevai/stella-engine.git
cd stella-engine
./run.sh --exec
```

If you don't want to use the script you can also do it manually:

```bash
git clone --recursive https://github.com/stefandevai/stella-engine.git
cd stella-engine
mkdir build
cd build
cmake ..
make
```

