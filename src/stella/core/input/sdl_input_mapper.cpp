#include "stella/core/input/sdl_input_mapper.hpp"

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
  #include <SDL.h>
  #include <SDL_timer.h>
#elif __APPLE__
  #include <SDL.h>
  #include <SDL_timer.h>
#else
  #include <SDL2/SDL.h>
  #include <SDL2/SDL_timer.h>
#endif
#undef main

namespace stella::core
{

  SDLInputMapper::SDLInputMapper()
  {

  }

  void SDLInputMapper::update (SDL_Event& event)
  {
    // Reset map each frame
    for (auto& entry : m_input_map)
    {
      entry.second = false;
    }

    while (SDL_PollEvent (&event))
    {
      switch (event.type)
      {
        case SDL_KEYDOWN:
        {
          switch (event.key.keysym.sym)
          {
            case SDL_SCANCODE_UP:
              m_input_map[InputEvent::KEYDOWN_ARROW_UP] = true;
              break;
            case SDL_SCANCODE_RIGHT:
              m_input_map[InputEvent::KEYDOWN_ARROW_RIGHT] = true;
              break;
            case SDL_SCANCODE_DOWN:
              m_input_map[InputEvent::KEYDOWN_ARROW_DOWN] = true;
              break;
            case SDL_SCANCODE_LEFT:
              m_input_map[InputEvent::KEYDOWN_ARROW_LEFT] = true;
              break;
            case SDL_SCANCODE_W:
              m_input_map[InputEvent::KEYDOWN_W] = true;
              break;
            case SDL_SCANCODE_D:
              m_input_map[InputEvent::KEYDOWN_D] = true;
              break;
            case SDL_SCANCODE_S:
              m_input_map[InputEvent::KEYDOWN_S] = true;
              break;
            case SDL_SCANCODE_A:
              m_input_map[InputEvent::KEYDOWN_A] = true;
              break;
            case SDL_SCANCODE_SPACE:
              m_input_map[InputEvent::KEYDOWN_SPACE] = true;
              break;
            default:
              break;
          }
          break;
        }

        case SDL_KEYUP:
        {
          switch (event.key.keysym.sym)
          {
            case SDL_SCANCODE_UP:
              m_input_map[InputEvent::KEYUP_ARROW_UP] = true;
              break;
            case SDL_SCANCODE_RIGHT:
              m_input_map[InputEvent::KEYUP_ARROW_RIGHT] = true;
              break;
            case SDL_SCANCODE_DOWN:
              m_input_map[InputEvent::KEYUP_ARROW_DOWN] = true;
              break;
            case SDL_SCANCODE_LEFT:
              m_input_map[InputEvent::KEYUP_ARROW_LEFT] = true;
              break;
            case SDL_SCANCODE_W:
              m_input_map[InputEvent::KEYUP_W] = true;
              break;
            case SDL_SCANCODE_D:
              m_input_map[InputEvent::KEYUP_D] = true;
              break;
            case SDL_SCANCODE_S:
              m_input_map[InputEvent::KEYUP_S] = true;
              break;
            case SDL_SCANCODE_A:
              m_input_map[InputEvent::KEYUP_A] = true;
              break;
            case SDL_SCANCODE_SPACE:
              m_input_map[InputEvent::KEYUP_SPACE] = true;
              break;
            default:
              break;
          }
          break;
        }

        case SDL_MOUSEBUTTONDOWN:
        {
          break;
        }

        case SDL_MOUSEBUTTONUP:
        {
          break;
        }

        case SDL_MOUSEMOTION:
        {
          break;
        }

        default:
          break;
      }
    }
  }

  const bool SDLInputMapper::query (const InputEvent event)
  {
    return m_input_map[event];
  }

  const bool SDLInputMapper::query (const std::initializer_list<InputEvent> events)
  {
    for (const auto event : events)
    {
      if (!m_input_map[event])
      {
        return false;
      }
    }

    return true;
  }

}

