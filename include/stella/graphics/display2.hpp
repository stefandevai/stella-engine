#pragma once

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

#include <string>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#ifdef STELLA_BUILD_EDITOR
namespace editor
{
class Editor;
} // namespace editor
#endif

namespace stella::graphics
{

  class Display
  {
  public:
    Display (const int width, const int height, const std::string& title);
    ~Display();

    void update();
    void render();
    void quit();
    void set_title (const std::string& title);
    void set_size (const int width, const int height);

    const bool is_running() const;
    const glm::vec2 get_size() const;
    const glm::vec2 get_viewport() const;
    const glm::vec2 get_dt() const;
    const glm::vec2 get_fps() const;

  private:
    // General fields
    int m_width;
    int m_height;
    std::string m_title;
    glm::vec3 m_clear_color{0.0f, 0.0f, 0.0f};

    // SDL related fields
    SDL_Window* m_window;
    SDL_GLContext m_gl_context;

#ifdef STELLA_BUILD_EDITOR
    friend class ::editor::Editor;

  #if __APPLE__
    // GL 3.2 Core + GLSL 150
    const char* m_glsl_version = "#version 150";
  #else
    // GL 3.0 + GLSL 130
    const char* m_glsl_version = "#version 130";
  #endif
#endif
  };

}
