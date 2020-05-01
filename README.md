![Stella Engine demo](/assets/img/stella-feb-2020.png)

# Stella Engine

[![Travis-CI Build Status](https://img.shields.io/travis/com/stefandevai/stella-engine?style=flat-square&logo=travis)](https://travis-ci.org/github/stefandevai/stella-engine)
[![AppVeyor Build Status](https://img.shields.io/appveyor/build/stefandevai/stella-engine?style=flat-square&logo=appveyor)](https://ci.appveyor.com/project/stefandevai/stella-engine)
[![License](https://img.shields.io/badge/license-GPL-blue?style=flat-square&logo=gnu&color=blueviolet)](./COPYING)

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

