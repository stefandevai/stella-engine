#include <iostream>
#include "display.h" 

namespace stella { namespace graphics {
  Display::Display(int width, int height, const std::string& title)
    : Width(width), Height(height), Title(title)
  {
    // GLFW initialization
    glfwInit();
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); 
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); 
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // Window creation
    this->Window = glfwCreateWindow(this->Width, this->Height, this->Title.c_str(), nullptr, nullptr);
    glfwMakeContextCurrent(this->Window);
    this->Running = true;
    
    // GLEW Initialization
    glewExperimental = GL_TRUE;
    glewInit();

    // OpenGL Viewport settings
    glViewport(0, 0, this->Width, this->Height);
  }

  Display::~Display()
  {
    // Terminating GLFW
    glfwTerminate();
  }

  bool Display::IsRunning()
  {
    return this->Running; 
  }

  void Display::Update()
  {
    this->updateInput();
    glfwSwapBuffers(this->Window);
  }

  void Display::updateInput()
  {
    glfwPollEvents();
    this->Running = !glfwWindowShouldClose(this->Window); 
  }
}}

