# Stella Engine

![Stella Engine demo](/assets/img/stella-feb-2020.png)

## Description:

A cross-platform 2D C++/OpenGL game engine in its early development phase.

## Current features:

- Batch Rendering
- Spritesheets
- Animations
- Particles
- Framebuffers
- Bitmap font rendering using awesome [usr_share Grafx2](https://opengameart.org/users/usrshare) fonts
- TTF font rendering
- Responsive audio streaming
- Platformer physics
- Simple grid physics
- Lua scripting
- Serialization
- ImGui GUI
- Custom map editor
- Inspector
- Natural Language Processing module

## Building:

The project uses cmake as its build system.

### Building an example on GNU/Linux or MacOS X:

```bash
git clone --recursive https://github.com/stefandevai/stella-engine.git
cd stella-engine
./helper.sh --build
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

