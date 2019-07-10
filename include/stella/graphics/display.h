#pragma once

#include <string>
#include <array>

//#include <SDL2/SDL.h>
//#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_timer.h>
#include <glm/glm.hpp>

//#include "stella/graphics/debug_gui.h"
// Set to 1 to disable vsync
#define DISABLE_VSYNC 0

//#define STELLA_BUILD_EDITOR
#ifdef STELLA_BUILD_EDITOR
#include "editor/editor_gui.h"
#endif

// Forward declarations
struct SDL_Window;
typedef void *SDL_GLContext;
typedef unsigned int GLuint;
typedef float GLfloat;

namespace stella {
namespace graphics {
class Display {
public:
  Display(GLuint width, GLuint height, const std::string &title);
  ~Display();
  bool IsRunning() const;
  void Update();
  void Clear();
  void SetClearColor(int r, int g, int b);
  void SetClearColor(GLfloat x, GLfloat y, GLfloat z);
  GLuint GetWidth() const;
  GLuint GetHeight() const;
  GLuint GetWindowWidth() const;
  GLuint GetWindowHeight() const;
  static GLfloat GetTime() { return (GLfloat)SDL_GetTicks(); }
  GLuint GetFrame() const { return Frame; }
  GLfloat GetDT() const { return DT; }
  GLfloat getFPS();
  static void GetMousePos(double &mx, double &my);
  static const unsigned char* GetGlVersion();
  static const unsigned char* GetGlRenderer();
	static bool IsKeyDown(int key);

#ifdef STELLA_BUILD_EDITOR
  void UpdateEditor(entt::registry &registry);
#endif

private:
  GLuint Width, Height, Frame, LastFrame;

  GLfloat LastTime, LastFPSCheck, DT = 0.0f;
  std::string Title;
  SDL_Window *Window;
  SDL_GLContext m_gl_context;
  bool Running;
  glm::vec3 ClearColor;

#ifdef STELLA_BUILD_EDITOR
  editor::EditorGui m_editor;
#endif

  void updateInput();
  void getDT();
};
} // namespace graphics
} // namespace stella
