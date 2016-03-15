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
    private:
      int Width, Height;
      std::string Title;
      GLFWwindow *Window;
      bool Running;

      void updateInput();
  };
}}

