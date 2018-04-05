#pragma once

#include <SDL2/SDL.h>
#include "stella/imgui/imgui.h"
#include "stella/imgui/imgui_impl_sdl_gl3.h"

class DebugGui {
  public:
    DebugGui();
    ~DebugGui();

    void Init(SDL_Window *window);
    void Update();
    void GetInput(SDL_Event &e);

  private:
    SDL_Window *Window;
};

