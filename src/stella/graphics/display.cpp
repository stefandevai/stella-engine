#include "stella/graphics/display.hpp"

#include "stella/graphics/opengl.hpp" // IWYU pragma: export

#include <spdlog/spdlog.h>
#include <sstream>
#include <stdexcept>

#if DISABLE_VSYNC == 1
  #ifdef __APPLE__
    #include <OpenCL/opencl.h>
    #include <OpenGL/OpenGL.h>
  #endif
#endif

namespace stella
{
namespace graphics
{
  double MouseX, MouseY;

  Display::Display (const unsigned int width, const unsigned int height, const std::string& title) : m_width (width), m_height (height), m_title (title)
  {
    // SDL initialization
    if (SDL_Init (SDL_INIT_VIDEO) < 0)
    {
      throw std::runtime_error ("It was not possible to initialize SDL2");
    }

#ifndef STELLA_BUILD_EDITOR
    SDL_ShowCursor (SDL_DISABLE);
#endif

    const SDL_WindowFlags window_flags = (SDL_WindowFlags) (SDL_WINDOW_RESIZABLE | SDL_WINDOW_MAXIMIZED | SDL_WINDOW_OPENGL);
    m_window = SDL_CreateWindow (m_title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_width, m_height, window_flags);

#if __APPLE__
    // Always required on Mac
    SDL_GL_SetAttribute (SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
    SDL_GL_SetAttribute (SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute (SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute (SDL_GL_CONTEXT_MINOR_VERSION, 3);
#else
    SDL_GL_SetAttribute (SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute (SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute (SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute (SDL_GL_CONTEXT_MINOR_VERSION, 0);
#endif

    SDL_GL_SetAttribute (SDL_GL_DOUBLEBUFFER, 1);
    m_gl_context = SDL_GL_CreateContext (m_window);
    SDL_GL_MakeCurrent (m_window, m_gl_context);
    SDL_GL_SetSwapInterval (1);

    m_running = true;

    if (!gladLoadGLLoader (SDL_GL_GetProcAddress))
    {
      spdlog::critical ("Failed to initialize GLAD");
    }

    // OpenGL Viewport settings
    glViewport (0, 0, m_width, m_height);

#ifndef STELLA_BUILD_EDITOR
    m_update_viewport_proportions();
#endif

    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable (GL_DEPTH_TEST);
  }

  Display::~Display()
  {
    SDL_GL_DeleteContext (m_gl_context);
    SDL_DestroyWindow (m_window);
    SDL_Quit();
  }

  const unsigned int Display::get_width() const { return m_width; }

  const unsigned int Display::get_height() const { return m_height; }

  const int Display::get_window_width() const
  {
    int width, height;
    SDL_GetWindowSize (m_window, &width, &height);
    return width;
  }

  const int Display::get_window_height() const
  {
    int width, height;
    SDL_GetWindowSize (m_window, &width, &height);
    return height;
  }

  const bool Display::is_running() const { return m_running; }

  void Display::update()
  {
    m_update_dt();
    m_update_input();
  }

  void Display::render()
  {
    SDL_GL_SwapWindow (m_window);
  }

  void Display::set_title (const std::string& title) { SDL_SetWindowTitle (m_window, title.c_str()); }

  void Display::set_size (const int width, const int height)
  {
    m_width  = width;
    m_height = height;

    auto flags = SDL_GetWindowFlags (m_window);

    // If the window is maximazed, don't resize it
    if (flags & SDL_WINDOW_MAXIMIZED)
    {
      return;
    }

    SDL_SetWindowSize (m_window, width, height);
  }

  void Display::set_clear_color (const float a, const float b, const float c)
  {
    m_clear_color.x = a;
    m_clear_color.y = b;
    m_clear_color.z = c;
  }

  void Display::clear()
  {
    /* glClearColor (m_clear_color.x, m_clear_color.y, m_clear_color.z, 1.0f); */
    glClearColor (0.5f, 0.5f, 0.5f, 1.0f);
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  }

  void Display::quit() { m_running = false; }

  void Display::m_update_input()
  {
    SDL_Event event;
    while (SDL_PollEvent (&event))
    {
      switch (event.type)
      {
        case SDL_QUIT:
        {
          m_running = false;
          break;
        }

        case SDL_WINDOWEVENT:
        {
          switch (event.window.event)
          {
            case SDL_WINDOWEVENT_RESIZED:
            case SDL_WINDOWEVENT_SIZE_CHANGED:
#ifndef STELLA_BUILD_EDITOR
              update_viewport();
#else
              reset_viewport();
#endif
              break;
          }
          break;
        }

        case SDL_MOUSEMOTION:
        {
          MouseX = event.motion.x;
          MouseY = event.motion.y;
          break;
        }
      }
    }
    m_event = event;
  }

  bool Display::is_key_down (int key)
  {
    const Uint8* keys = SDL_GetKeyboardState (NULL);
    return keys[key];
  }

  void Display::m_update_dt()
  {
    const unsigned int current_time  = SDL_GetTicks();
    m_dt                = current_time - m_last_time;
    m_last_time         = current_time;
  }

  void Display::get_mouse_pos (double& mx, double& my)
  {
    mx = MouseX;
    my = MouseY;
  }

  const float Display::get_fps() const
  {
    return (1.0f / (static_cast<float>(m_dt) / 1000.0f));
  }

  void Display::reset_viewport()
  {
    glViewport (0, 0, m_width, m_height);
  }

  void Display::update_viewport()
  {
    int width, height;
    SDL_GetWindowSize (m_window, &width, &height);
    glViewport (0, 0, width, height);
    m_update_viewport_proportions();
  }

  void Display::m_update_viewport_proportions()
  {
    int vpcoords[4];
    glGetIntegerv (GL_VIEWPORT, vpcoords);

    const int width  = vpcoords[2];
    const int height = vpcoords[3];

    // 16/9 = 1.77777. Therefore, we check if the new proportions are greater or lower than that
    const auto factor = width / static_cast<float>(height);
    if (factor > 1.78f)
    { // Height is max and width is adjusted
      const int new_width = height * 1.77777f;
      const int left      = width - new_width;
      glViewport (left / 2, 0, new_width, height);
    }
    else if (factor < 1.77f)
    { // Width is max and height is adjusted
      const int new_height = (int) width / 1.77f;
      const int left       = height - new_height;
      glViewport (0, left / 2, width, new_height);
    }
  }
} // namespace graphics
} // namespace stella
