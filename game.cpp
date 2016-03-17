#include <iostream>
#include <vector>
#include <cmath>
#include "glm/glm/glm.hpp"
#include "glm/glm/gtc/matrix_transform.hpp"
#include "src/stella.h"

int main(int argc, const char *argv[])
{
  using namespace stella;
  using namespace graphics;

  Display display(640, 480, "Stella");
  display.SetClearColor(22, 38, 47);

  Shader shader("assets/shaders/basic_shader.vsh", "assets/shaders/basic_shader.fsh");

  glm::mat4 proj = glm::ortho(0.0f, (GLfloat)display.GetWidth(), (GLfloat)display.GetHeight(), 0.0f, -1.0f, 1.0f);
  shader.Enable();
  shader.SetMat4("proj", proj);
  shader.Disable();

  Texture texture("assets/gfx/sprites/guanaco.png");
  Renderer renderer(&texture);
  std::vector<Sprite*> sprites;
  srand(47);
  for (int i = 0; i < 11; i++)
    for (int j = 0; j < 15; j++)
    {
      Sprite *sprite = new Sprite(j*32 + 68, i*32 + 68, texture, 0);
      sprite->SetColor(rand()%(200-30 + 1) + 30, rand()%(200-160 + 1) + 160, rand()%(240-20 + 1) + 20);
      sprites.push_back(sprite);
    }

  glm::mat4 model;

  while (display.IsRunning())
  {
    display.Clear();
    
    shader.Enable();
    renderer.Begin();
    for (auto i : sprites)
      renderer.Submit(*i);
    renderer.End();
    renderer.Draw();

    model = glm::translate(model, glm::vec3(display.GetWidth()/2.0f, display.GetHeight()/2.0f, 0.0f));
    model = glm::rotate(model, glm::radians(display.GetDT() * 30.0f * cosf(display.GetTime())), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::translate(model, glm::vec3(display.GetWidth()/-2.0f, display.GetHeight()/-2.0f, 0.0f));

    shader.SetMat4("model", model);

    display.Update();
  }

  shader.Disable();
  for (auto i : sprites)
    delete i;

  return 0;
}

