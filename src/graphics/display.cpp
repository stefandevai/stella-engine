#include <sstream>
#include "../../Dependencies/glm/glm/glm.hpp"
#include "display.h" 

#include <iostream>

namespace stella { namespace graphics {
  GLuint KeyPressed, KeyReleased;
  GLboolean KeyPress = GL_FALSE, KeyRelease = GL_FALSE;
  double MouseX, MouseY;

  Display::Display(GLuint width, GLuint height, const std::string& title, GLboolean (&keys)[1024])
    : Width(width), Height(height), Title(title), Keys(keys)
  {
    // GLFW initialization
    if (!glfwInit())
      std::cout << "Failed to initialize GLFW." << std::endl;
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); 
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); 
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // Window creation
    this->Window = glfwCreateWindow(this->Width, this->Height, this->Title.c_str(), nullptr, nullptr);
    if (this->Window == nullptr) std::cout << "GLFW Error: It was not possible to create a Window." << std::endl;
    glfwMakeContextCurrent(this->Window);
    this->Running = true;

    // Set initial value for Frame
    this->Frame = 1; // Setting as 1 to avoid division by 0
    this->LastFPSCheck = this->LastTime = glfwGetTime();
    this->LastFrame = 0;
    
    // Input callback
    glfwSetKeyCallback(this->Window, this->inputCallback);
    glfwSetCursorPosCallback(this->Window, this->mouseCallback);
    
    // GLEW Initialization
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
      std::cout << "Failed to initialize GLEW" << std::endl;

    // OpenGL Viewport settings
    glViewport(0, 0, this->Width, this->Height);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Set default Clear Color
    this->ClearColor = glm::vec3(0.0f, 0.0f, 0.0f);
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
    this->getDT();
    this->Frame++;
    if (this->Frame >= 10000000)
      this->Frame = 0;
    if (this->Frame%30 == 0)
    {
      std::stringstream compo;
      compo << Title << " (" << this->getFPS() << " FPS)";
      glfwSetWindowTitle(this->Window, compo.str().c_str());
    }

    this->updateInput();
    glfwSwapBuffers(this->Window);
  }

  void Display::SetClearColor(int r, int g, int b)
  {
    this->ClearColor.x = r/255.0f;
    this->ClearColor.y = g/255.0f;
    this->ClearColor.z = b/255.0f;
  }

  void Display::SetClearColor(GLfloat x, GLfloat y, GLfloat z)
  {
    if (x > 1.0f) x = 1.0f;
    if (y > 1.0f) y = 1.0f;
    if (z > 1.0f) z = 1.0f;
    
    this->ClearColor.x = x;
    this->ClearColor.y = y;
    this->ClearColor.z = z;
  }

  void Display::Clear()
  {
    glClearColor(this->ClearColor.x, this->ClearColor.y, this->ClearColor.z, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
  }

  void Display::updateInput()
  {
    glfwPollEvents();
    this->Running = !glfwWindowShouldClose(this->Window); 
    
    if (KeyPress)
    {
      KeyPress = GL_FALSE;
      this->Keys[KeyPressed] = GL_TRUE;
    }
    if (KeyRelease)
    {
      KeyRelease = GL_FALSE;
      this->Keys[KeyReleased] = GL_FALSE;
    }
  }

  void Display::getDT()
  {
    GLfloat currentTime = glfwGetTime();
    this->DT = currentTime - this->LastTime;
    this->LastTime = currentTime;
  }

  void Display::GetMousePos(double &mx, double &my)
  {
    mx = MouseX;
    my = MouseY;
  }

  GLfloat Display::getFPS()
  {
    GLuint currentFrame = this->Frame;
    GLuint deltaFrame = currentFrame - this->LastFrame;
    this->LastFrame = currentFrame;

    GLfloat currentTime = glfwGetTime();
    GLfloat deltaTime = currentTime - this->LastFPSCheck;
    this->LastFPSCheck = currentTime;

    return deltaFrame / deltaTime;
  }

  void Display::inputCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
  {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
      glfwSetWindowShouldClose(window, GL_TRUE);
    if (key >= 0 && key < 1024)
    {
      if (action == GLFW_PRESS)
      {
        KeyPress = GL_TRUE;
        KeyPressed = key;
      }
      else if (action == GLFW_RELEASE)
      {
        KeyRelease = GL_TRUE;
        KeyReleased = key;
      } 
    }
  }

  void Display::mouseCallback(GLFWwindow* window, double xpos, double ypos)
  {
    MouseX = xpos;
    MouseY = ypos;
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    if (MouseX < 0) MouseX = 0;
    else if (MouseX > width) MouseX = width;
    if (MouseY < 0) MouseY = 0;
    else if (MouseY > height) MouseY = height;
  }
}}

