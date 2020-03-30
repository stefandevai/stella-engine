#include "stella/graphics/display.hpp"

#include "stella/graphics/opengl.hpp"

#include <iostream>
#include <sstream>

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
    : Width (width), Height (height), Title (title)
  {
    // SDL initialization
    if (SDL_Init (SDL_INIT_VIDEO) < 0)
    {
      std::cout << "It was not possible to initialize SDL2" << std::endl;
    }

#ifdef STELLA_BUILD_EDITOR
    // const SDL_WindowFlags window_flags =
    // (SDL_WindowFlags) (SDL_WINDOW_MAXIMIZED | SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI);
    const SDL_WindowFlags window_flags =
        (SDL_WindowFlags) (SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI);
#else
    const SDL_WindowFlags window_flags = (SDL_WindowFlags) (SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI);
    SDL_ShowCursor (SDL_DISABLE);
#endif
    this->Window = SDL_CreateWindow (
        this->Title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, this->Width, this->Height, window_flags);
    

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
    m_gl_context = SDL_GL_CreateContext (this->Window);
    SDL_GL_MakeCurrent (this->Window, m_gl_context);
    SDL_GL_SetSwapInterval (1);

    this->Running = true;

    // Set initial value for Frame
    this->Frame        = 1; // Setting as 1 to avoid division by 0
    this->LastFPSCheck = this->LastTime = (float) SDL_GetTicks() / 1000.0f;
    this->LastFrame                     = 0;

    if (!gladLoadGLLoader (SDL_GL_GetProcAddress))
    {
      std::cout << "Failed to initialize GLAD" << std::endl;
    }

    // OpenGL Viewport settings
    glViewport (0, 0, this->Width, this->Height);

#ifndef STELLA_BUILD_EDITOR
    m_check_viewport_proportions();
#endif

    glEnable (GL_BLEND);
    // glDisable(GL_DEPTH_TEST);
    // glEnable (GL_DEPTH_TEST);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Set default Clear Color
    this->ClearColor = glm::vec3 (0.0f, 0.0f, 0.0f);
  }

  Display::~Display()
  {
    SDL_GL_DeleteContext (m_gl_context);
    SDL_DestroyWindow (this->Window);
    SDL_Quit();
  }

  GLuint Display::GetWidth() const { return this->Width; }

  GLuint Display::GetWindowWidth() const
  {
    int width, height;
    SDL_GetWindowSize (this->Window, &width, &height);
    return width;
  }

  GLuint Display::GetHeight() const { return this->Height; }

  GLuint Display::GetWindowHeight() const
  {
    int width, height;
    SDL_GetWindowSize (this->Window, &width, &height);
    return height;
  }

  bool Display::IsRunning() const { return this->Running; }

  void Display::Update()
  {
#if DISABLE_VSYNC == 1
  #ifdef __APPLE__
    GLint vsync       = 0;
    CGLContextObj ctx = CGLGetCurrentContext();
    CGLSetParameter (ctx, kCGLCPSwapInterval, &vsync);
  #endif

    // Print FPS
    if (this->Frame % 120 == 0)
    {
      std::cout << this->getFPS() << '\n';
    }
#endif

    this->getDT();
    this->Frame++;
    if (this->Frame >= 10000000)
    {
      this->Frame = 0;
    }

    this->updateInput();
    SDL_GL_SwapWindow (this->Window);
  }

  void Display::SetClearColor (int r, int g, int b)
  {
    this->ClearColor.x = r / 255.0f;
    this->ClearColor.y = g / 255.0f;
    this->ClearColor.z = b / 255.0f;
  }

  void Display::SetClearColor (GLfloat x, GLfloat y, GLfloat z)
  {
    if (x > 1.0f)
      x = 1.0f;
    if (y > 1.0f)
      y = 1.0f;
    if (z > 1.0f)
      z = 1.0f;

    this->ClearColor.x = x;
    this->ClearColor.y = y;
    this->ClearColor.z = z;
  }

  void Display::Clear()
  {
    glClearColor (this->ClearColor.x, this->ClearColor.y, this->ClearColor.z, 1.0f);
    // glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClear (GL_COLOR_BUFFER_BIT);
    // glEnable(GL_DEPTH_TEST);
  }

  void Display::updateInput()
  {
    SDL_Event event;
    while (SDL_PollEvent (&event))
    {
      switch (event.type)
      {
        case SDL_QUIT:
          this->Running = false;
          break;
        case SDL_KEYDOWN:
          {
          const Uint8* state = SDL_GetKeyboardState (nullptr);
          if (state[SDL_SCANCODE_LCTRL] && state[SDL_SCANCODE_Q])
          {
            this->Running = false;
          }
          break;
          }
        case SDL_WINDOWEVENT:
          switch (event.window.event)
          {
            case SDL_WINDOWEVENT_RESIZED:
#ifndef STELLA_BUILD_EDITOR
              glViewport (0, 0, GetWindowWidth(), GetWindowHeight());
              m_check_viewport_proportions();
//#else
//              glViewport (GetWindowWidth() - Width, GetWindowHeight() - Height - 23, this->Width, this->Height);
#endif
              break;
            case SDL_WINDOWEVENT_SIZE_CHANGED:
#ifndef STELLA_BUILD_EDITOR
              glViewport (0, 0, GetWindowWidth(), GetWindowHeight());
              m_check_viewport_proportions();
//#else
//              glViewport (GetWindowWidth() - Width, GetWindowHeight() - Height - 23, this->Width, this->Height);
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

  bool Display::IsKeyDown (int key)
  {
    const Uint8* keys = SDL_GetKeyboardState (NULL);
    return keys[key];
  }

  void Display::getDT()
  {
    GLfloat currentTime = (float) SDL_GetTicks() / 1000.0f;
    this->DT            = currentTime - this->LastTime;
    this->LastTime      = currentTime;
  }

  void Display::GetMousePos (double& mx, double& my)
  {
    mx = MouseX;
    my = MouseY;
  }

  const unsigned char* Display::GetGlVersion() { return glGetString (GL_VERSION); }

  const unsigned char* Display::GetGlRenderer() { return glGetString (GL_RENDERER); }

  GLfloat Display::getFPS()
  {
    GLuint currentFrame = this->Frame;
    GLuint deltaFrame   = currentFrame - this->LastFrame;
    this->LastFrame     = currentFrame;

    GLfloat currentTime = (float) SDL_GetTicks() / 1000.0f;
    GLfloat deltaTime   = currentTime - this->LastFPSCheck;
    this->LastFPSCheck  = currentTime;

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
    int newwidth = height * 1.77777f;
    int left     = width - newwidth;
    // std::cout << newwidth << std::endl;
    glViewport (left / 2, 0, newwidth, height);
  }
  else if (width / (float) height < 1.77f)
  { // Width is max and height is adjusted
    int newheight = (int) width / 1.77f;
    int left      = height - newheight;
    // std::cout << newheight << std::endl;
    glViewport (0, left / 2, width, newheight);
  }
}
} // namespace graphics
} // namespace stella
