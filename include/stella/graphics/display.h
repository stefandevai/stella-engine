#pragma once

#include <string>
#include <array>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

namespace stella {
namespace graphics {
class Display {
public:
  Display(GLuint width, GLuint height, const std::string &title);
  ~Display();
  bool IsRunning();
  void Update();
  void Clear();
  void SetClearColor(int r, int g, int b);
  void SetClearColor(GLfloat x, GLfloat y, GLfloat z);
  GLuint GetWidth();
  GLuint GetHeight();
  GLfloat GetTime() { return (GLfloat)glfwGetTime(); }
  GLuint GetFrame() { return Frame; }
  GLfloat GetDT() { return DT; }
  GLfloat getFPS();
  void GetMousePos(double &mx, double &my);
	bool IsKeyDown(int key);

private:
  GLuint Width, Height, Frame, LastFrame;

  GLfloat LastTime, LastFPSCheck, DT;
  std::string Title;
  GLFWwindow *Window;
  bool Running;
  glm::vec3 ClearColor;

  void updateInput();
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
