#include <iostream>
#include "glm/glm/glm.hpp"
#include "src/stella.h"

int main(int argc, const char *argv[])
{
  using namespace stella;
  using namespace graphics;

  Display display(640, 480, "Stella");
  display.SetClearColor(204, 102, 51);

  Shader shader("assets/shaders/basic_shader.vsh", "assets/shaders/basic_shader.fsh");
  
  GLfloat vertices[] = 
  {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f
  };

  GLuint VAO, VBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);

  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  while (display.IsRunning())
  {
    display.Clear();

    shader.Enable();
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);

    display.Update();
  }
  return 0;
}

