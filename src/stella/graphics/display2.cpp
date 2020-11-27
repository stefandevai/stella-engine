#include "stella/graphics/display2.hpp"
#include "stella/graphics/opengl.hpp" // IWYU pragma: export
#include <spdlog/spdlog.h>

namespace stella::graphics
{

  Display::Display (const int width, const int height, const std::string& title)
    : m_width (width), m_height (height), m_title (title)
  {
    // Initialization
    auto error = SDL_Init (SDL_INIT_VIDEO);

    if (error < 0)
    {
      throw std::runtime_error ("It was not possible to initialize SDL2");
    }

    // Window creation
#ifndef STELLA_BUILD_EDITOR
    SDL_ShowCursor (SDL_DISABLE);
#endif

    const SDL_WindowFlags window_flags = (SDL_WindowFlags) (SDL_WINDOW_RESIZABLE | SDL_WINDOW_MAXIMIZED | SDL_WINDOW_OPENGL);
    m_window = SDL_CreateWindow (m_title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_width, m_height, window_flags);

    // OpenGL context creation
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

    if (!gladLoadGLLoader (SDL_GL_GetProcAddress))
    {
      spdlog::critical ("Failed to initialize GLAD");
    }

    glViewport (0, 0, m_width, m_height);

  }

  Display::~Display()
  {
    SDL_GL_DeleteContext (m_gl_context);
    SDL_DestroyWindow (m_window);
    SDL_Quit();
  }

}
