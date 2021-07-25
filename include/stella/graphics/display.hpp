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
#include <glm/glm.hpp>
#include <string>

/* #ifdef STELLA_BUILD_EDITOR */
namespace editor
{
class Editor;
} // namespace editor
/* #endif */

namespace stella
{
namespace graphics
{
  class Display
  {
  public:
    Display (const unsigned int width, const unsigned int height, const std::string& title);
    ~Display();
    const bool is_running() const;
    void update();
    void render();
    void clear();
    void quit();
    void set_title (const std::string& title);
    void set_size (const int width, const int height);
    void set_clear_color (const float a, const float b, const float c);
    const unsigned int get_width() const;
    const unsigned int get_height() const;
    const int get_window_width() const;
    const int get_window_height() const;
    static float get_time() { return (float) SDL_GetTicks(); }
    const float get_dt() const { return (static_cast<float> (m_dt) / 1000.0f); }
    const float get_fps() const;
    static void get_mouse_pos (double& mx, double& my);
    static bool is_key_down (int key);
    void reset_viewport();
    void update_viewport();

  private:
    SDL_Window* m_window;
    SDL_GLContext m_gl_context;
    SDL_Event m_event;
    unsigned int m_width;
    unsigned int m_height;
    std::string m_title;
    unsigned int m_dt        = 0;
    unsigned int m_last_time = 0;
    bool m_running           = false;
    glm::vec3 m_clear_color{0.0f, 0.0f, 0.0f};

    /* #ifdef STELLA_BUILD_EDITOR */
    friend class ::editor::Editor;

#if __APPLE__
    // GL 3.2 Core + GLSL 150
    const char* m_glsl_version = "#version 150";
#else
    // GL 3.0 + GLSL 130
    const char* m_glsl_version = "#version 130";
#endif
    /* #endif */

  private:
    void m_update_input();
    void m_update_dt();
    void m_update_viewport_proportions();
  };
} // namespace graphics
} // namespace stella
