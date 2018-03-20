#pragma once

#include <string>
#include <array>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

namespace stella {
namespace graphics {
class Display {
public:
  Display(GLuint width, GLuint height, const std::string &title,
          std::array<bool, 1024> &keys);
  ~Display();
  bool IsRunning();
  void Update();
  void Clear();
  void SetClearColor(int r, int g, int b);
  void SetClearColor(GLfloat x, GLfloat y, GLfloat z);
  GLuint GetWidth() { return Width; }
  GLuint GetHeight() { return Height; }
  GLfloat GetTime() { return (GLfloat)glfwGetTime(); }
  GLuint GetFrame() { return Frame; }
  GLfloat GetDT() { return DT; }
  void GetMousePos(double &mx, double &my);
	bool IsKeyDown(int key);

private:
  GLuint Width, Height, Frame, LastFrame;
  GLfloat LastTime, LastFPSCheck, DT;
  std::string Title;
  GLFWwindow *Window;
  bool Running;
  glm::vec3 ClearColor;
  //GLboolean (&Keys)[1024];
	std::array<bool, 1024> &Keys;

  void updateInput();
  GLfloat getFPS();
  void getDT();
  void checkViewportProportions();
  static void inputCallback(GLFWwindow *window, int key, int scancode,
                            int action, int mode);
  static void mouseCallback(GLFWwindow *window, double xpos, double ypos);
  static void windowSizeCallback(GLFWwindow *window, int width, int height);
  static void errorCallback(int error, const char *description);
};
} // namespace graphics
} // namespace stella
