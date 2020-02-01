#pragma once

#include <array>
#include <string>

//#include <SDL2/SDL.h>
//#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#undef main
#include <glm/glm.hpp>

#ifdef STELLA_BUILD_EDITOR
  #include "editor/editor_gui.h"
#endif

// Set to 1 to disable vsync
#define DISABLE_VSYNC 0

// Forward declarations
struct SDL_Window;
typedef void* SDL_GLContext;
typedef unsigned int GLuint;
typedef float GLfloat;

namespace stella
{
namespace graphics
{
  class Display
  {
  public:
    Display (GLuint width, GLuint height, const std::string& title);
    ~Display();
    bool IsRunning() const;
    void Update();
    void Clear();
    void SetClearColor (int r, int g, int b);
    void SetClearColor (GLfloat x, GLfloat y, GLfloat z);
    GLuint GetWidth() const;
    GLuint GetHeight() const;
    GLuint GetWindowWidth() const;
    GLuint GetWindowHeight() const;
    static GLfloat GetTime() { return (GLfloat) SDL_GetTicks(); }
    GLuint GetFrame() const { return Frame; }
    GLfloat GetDT() const { return DT; }
    GLfloat getFPS();
    static void GetMousePos (double& mx, double& my);
    static const unsigned char* GetGlVersion();
    static const unsigned char* GetGlRenderer();
    static bool IsKeyDown (int key);

#ifdef STELLA_BUILD_EDITOR
    // void UpdateEditor (entt::registry& registry);
    // inline void SetEditor (editor::EditorGui* editor)
    // {
    //   this->editor = editor;
    //   this->editor->init (this->Window, m_gl_context, m_glsl_version);
    // }

    
#endif

  private:
    GLuint Width, Height, Frame, LastFrame;

    GLfloat LastTime, LastFPSCheck, DT = 0.0f;
    std::string Title;
    SDL_Window* Window;
    SDL_GLContext m_gl_context;
    bool Running;
    glm::vec3 ClearColor;
#ifdef STELLA_BUILD_EDITOR
    friend class stella::editor::EditorGui;
    // editor::EditorGui* editor = nullptr;
#endif

  #if __APPLE__
    // GL 3.2 Core + GLSL 150
    const char* m_glsl_version = "#version 150";
  #else
    // GL 3.0 + GLSL 130
    const char* m_glsl_version = "#version 130";
  #endif


    void updateInput();
    void getDT();
  };
} // namespace graphics
} // namespace stella
