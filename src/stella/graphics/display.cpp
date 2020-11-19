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

  Display::Display (GLuint width, GLuint height, const std::string& title)
    : m_width (width), m_height (height), m_title (title)
  {
    // SDL initialization
    if (SDL_Init (SDL_INIT_VIDEO) < 0)
    {
      throw std::runtime_error("It was not possible to initialize SDL2");
    }

    const SDL_WindowFlags window_flags = (SDL_WindowFlags) (SDL_WINDOW_RESIZABLE | SDL_WINDOW_MAXIMIZED | SDL_WINDOW_OPENGL);

#ifndef STELLA_BUILD_EDITOR
    SDL_ShowCursor (SDL_DISABLE);
#endif

    m_window = SDL_CreateWindow (
        m_title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_width, m_height, window_flags);

#if __APPLE__
    SDL_GL_SetAttribute (SDL_GL_CONTEXT_FLAGS,
                         SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG); // Always required on Mac
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

    // Set initial value for Frame
    m_frame        = 1; // Setting as 1 to avoid division by 0
    m_last_fps_check = m_last_time = (float) SDL_GetTicks() / 1000.0f;
    m_last_frame                     = 0;

    if (!gladLoadGLLoader (SDL_GL_GetProcAddress))
    {
      spdlog::critical("Failed to initialize GLAD");
    }

    // OpenGL Viewport settings
    glViewport (0, 0, m_width, m_height);

#ifndef STELLA_BUILD_EDITOR
    m_check_viewport_proportions();
#endif

    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // glDisable(GL_DEPTH_TEST);
    glEnable (GL_DEPTH_TEST);
  }

  Display::~Display()
  {
    SDL_GL_DeleteContext (m_gl_context);
    SDL_DestroyWindow (m_window);
    SDL_Quit();
  }

  GLuint Display::get_width() const { return m_width; }

  GLuint Display::get_window_width() const
  {
    int width, height;
    SDL_GetWindowSize (m_window, &width, &height);
    return width;
  }

  GLuint Display::get_height() const { return m_height; }

  GLuint Display::get_window_height() const
  {
    int width, height;
    SDL_GetWindowSize (m_window, &width, &height);
    return height;
  }

  bool Display::is_running() const { return m_running; }

  void Display::update()
  {
#if DISABLE_VSYNC == 1
  #ifdef __APPLE__
    GLint vsync       = 0;
    CGLContextObj ctx = CGLGetCurrentContext();
    CGLSetParameter (ctx, kCGLCPSwapInterval, &vsync);
  #endif

    // Print FPS
    if (m_frame % 120 == 0)
    {
      spdlog::debug(this->get_fps());
    }
#endif

    this->m_get_dt();
    m_frame++;
    //if (m_frame >= 10000000)
    //{
      //m_frame = 0;
    //}

    this->m_update_input();
    SDL_GL_SwapWindow (m_window);
  }

  void Display::set_title (const std::string& title)
  {
    SDL_SetWindowTitle(m_window, title.c_str());
  }

  void Display::set_size (const int width, const int height)
  {
    m_width = width;
    m_height = height;

    auto flags = SDL_GetWindowFlags(m_window);

    // If the window is maximazed, don't resize it
    if (flags & SDL_WINDOW_MAXIMIZED)
    {
      return;
    }

    SDL_SetWindowSize(m_window, width, height);
  }

  void Display::set_clear_color (int r, int g, int b)
  {
    m_clear_color.x = r / 255.0f;
    m_clear_color.y = g / 255.0f;
    m_clear_color.z = b / 255.0f;
  }

  void Display::set_clear_color (GLfloat x, GLfloat y, GLfloat z)
  {
    if (x > 1.0f)
      x = 1.0f;
    if (y > 1.0f)
      y = 1.0f;
    if (z > 1.0f)
      z = 1.0f;

    m_clear_color.x = x;
    m_clear_color.y = y;
    m_clear_color.z = z;
  }

  void Display::clear()
  {
    glClearColor (m_clear_color.x, m_clear_color.y, m_clear_color.z, 1.0f);
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // glClear (GL_COLOR_BUFFER_BIT);
    // glEnable(GL_DEPTH_TEST);
  }

  void Display::quit()
  {
    m_running = false;
  }

  void Display::m_update_input()
  {
    SDL_Event event;
    while (SDL_PollEvent (&event))
    {
      switch (event.type)
      {
        case SDL_QUIT:
          m_running = false;
          break;
        case SDL_KEYDOWN:
        {
          const Uint8* state = SDL_GetKeyboardState (nullptr);
          if (state[SDL_SCANCODE_LCTRL] && state[SDL_SCANCODE_Q])
          {
            m_running = false;
          }
          break;
        }
        case SDL_WINDOWEVENT:
          switch (event.window.event)
          {
            case SDL_WINDOWEVENT_RESIZED:
            case SDL_WINDOWEVENT_SIZE_CHANGED:
#ifndef STELLA_BUILD_EDITOR
              glViewport (0, 0, get_window_width(), get_window_height());
              m_check_viewport_proportions();
#else
              glViewport (0, 0, m_width, m_height);
#endif
              break;
          }
          break;
        case SDL_MOUSEMOTION:
          MouseX = event.motion.x;
          MouseY = event.motion.y;
          break;
      }
    }
    m_event = event;
  }

  bool Display::is_key_down (int key)
  {
    const Uint8* keys = SDL_GetKeyboardState (NULL);
    return keys[key];
  }

  void Display::m_get_dt()
  {
    GLfloat currentTime = (float) SDL_GetTicks() / 1000.0f;
    m_dt            = currentTime - m_last_time;
    m_last_time      = currentTime;
  }

  void Display::get_mouse_pos (double& mx, double& my)
  {
    mx = MouseX;
    my = MouseY;
  }

  const unsigned char* Display::get_gl_version() { return glGetString (GL_VERSION); }

  const unsigned char* Display::get_gl_renderer() { return glGetString (GL_RENDERER); }

  GLfloat Display::get_fps()
  {
    GLuint currentFrame = m_frame;
    GLuint deltaFrame   = currentFrame - m_last_frame;
    m_last_frame     = currentFrame;

    GLfloat currentTime = (float) SDL_GetTicks() / 1000.0f;
    GLfloat deltaTime   = currentTime - m_last_fps_check;
    m_last_fps_check  = currentTime;

    return deltaFrame / deltaTime;
  }

  void Display::m_check_viewport_proportions()
  {
    int width, height;
    int vpcoords[4];
    glGetIntegerv (GL_VIEWPORT, vpcoords);

    width  = vpcoords[2];
    height = vpcoords[3];

    // 16/9 = 1.77777. Therefore, we check if the new proportions are greater or
    // lower than that
    if (width / (float) height > 1.78f)
    { // Height is max and width is adjusted
      int new_width = height * 1.77777f;
      int left     = width - new_width;
      glViewport (left / 2, 0, new_width, height);
    }
    else if (width / (float) height < 1.77f)
    { // Width is max and height is adjusted
      int new_height = (int) width / 1.77f;
      int left      = height - new_height;
      glViewport (0, left / 2, width, new_height);
    }
  }
} // namespace graphics
} // namespace stella
