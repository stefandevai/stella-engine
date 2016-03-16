#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace stella { namespace graphics {
  class Display {
    public:
      Display(GLuint width, GLuint height, const std::string& title);
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

    private:
      GLuint Width, Height, Frame, LastFrame;
      GLfloat LastTime, LastFPSCheck, DT;
      std::string Title;
      GLFWwindow *Window;
      bool Running;
      glm::vec3 ClearColor;

      void updateInput();
      GLfloat getFPS();
      void getDT();
      static void inputCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
  };
}}

