#include <iostream>
#include "glm/glm/glm.hpp"
#include "glm/glm/gtc/matrix_transform.hpp"
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
    0.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f,

    1.0f,  1.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
     0.0f,  1.0f, 0.0f
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
  
  glm::mat4 proj = glm::ortho(0.0f, (GLfloat)display.GetWidth(), (GLfloat)display.GetHeight(), 0.0f, -1.0f, 1.0f);
  glm::mat4 model;
  model = glm::scale(model, glm::vec3(64.0f, 64.0f, 1.0f));
  glm::mat4 view;
  view = glm::translate(view, glm::vec3(display.GetWidth()/2.0f - 32.0f, display.GetHeight()/2.0f - 32.0f, 0.0f));
  
  shader.Enable();
  shader.SetMat4("model", model);
  shader.SetMat4("view", view);
  shader.SetMat4("proj", proj);
  shader.Disable();

  while (display.IsRunning())
  {
    display.Clear();

    model = glm::translate(model, glm::vec3(0.5f, 0.5f, 0.0f));
    model = glm::rotate(model, glm::radians(display.GetDT()*50.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::translate(model, glm::vec3(-0.5f, -0.5f, 0.0f));
    //std::cout << display.GetTime() << std::endl;

    shader.Enable();
    shader.SetMat4("model", model);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);

    display.Update();
  }

  shader.Disable();
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);

  return 0;
}

