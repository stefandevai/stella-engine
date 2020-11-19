#pragma once

#include <array>
#include <string>

//#include <SDL2/SDL.h>
//#include <SDL2/SDL_opengl.h>
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

// Set to 1 to disable vsync
#define DISABLE_VSYNC 0

// Forward declarations
struct SDL_Window;
typedef void* SDL_GLContext;
typedef unsigned int GLuint;
typedef float GLfloat;

#ifdef STELLA_BUILD_EDITOR
namespace editor
{
  class Editor;
} // namespace stella
#endif

namespace stella
{
namespace graphics
{
  class Display
  {
  public:
    Display (GLuint width, GLuint height, const std::string& title);
    ~Display();
    bool is_running() const;
    void update();
    void clear();
    void quit();
    void set_title (const std::string& title);
    void set_size (const int width, const int height);
    void set_clear_color (int r, int g, int b);
    void set_clear_color (GLfloat x, GLfloat y, GLfloat z);
    GLuint get_width() const;
    GLuint get_height() const;
    GLuint get_window_width() const;
    GLuint get_window_height() const;
    static GLfloat get_time() { return (GLfloat) SDL_GetTicks(); }
    GLuint get_frame() const { return m_frame; }
    GLfloat get_dt() const { return m_dt; }
    GLfloat get_fps();
    static void get_mouse_pos (double& mx, double& my);
    static const unsigned char* get_gl_version();
    static const unsigned char* get_gl_renderer();
    static bool is_key_down (int key);

  private:
    GLuint m_width, m_height, m_frame, m_last_frame;
    GLfloat m_last_time, m_last_fps_check, m_dt = 0.0f;
    std::string m_title;
    SDL_Window* m_window;
    SDL_GLContext m_gl_context;
    bool m_running;
    glm::vec3 m_clear_color{0.f, 0.f, 0.f};
    SDL_Event m_event;

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

    void m_update_input();
    void m_get_dt();
    void m_check_viewport_proportions();
  };
} // namespace graphics
} // namespace stella
