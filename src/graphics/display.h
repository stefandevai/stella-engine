#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace stella { namespace graphics {
  class Display {
    public:
      Display(int width, int height, const std::string& title);
      ~Display();
      bool IsRunning();
      void Update();
      void Clear();
      void SetClearColor(int r, int g, int b);

    private:
      int Width, Height;
      std::string Title;
      GLFWwindow *Window;
      bool Running;
      glm::vec3 ClearColor;

      void updateInput();
      static void inputCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
  };
}}

