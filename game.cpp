#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>
#include <map>
#include <cmath>
#include "Dependencies/glm/glm/glm.hpp"
#include "Dependencies/glm/glm/gtc/matrix_transform.hpp"
#include "src/stella.h"

int main(int argc, char *argv[])
{
  using namespace stella;
  using namespace graphics;
  using namespace audio;

  GLboolean Keys[1024] = { 0 };

  Display display(800, 600, "Stella", Keys);
  display.SetClearColor(22, 38, 47);

  // TODO Move this block to init on singleton class
  GLint tex_ids[] =
  {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11
  };

  Shader shader("assets/shaders/basic_shader.vsh", "assets/shaders/basic_shader.fsh");
  glm::mat4 proj = glm::ortho(0.0f, (GLfloat)display.GetWidth(), (GLfloat)display.GetHeight(), 0.0f, -1.0f, 1.0f);

  SceneLayer layer(&shader, proj);

  shader.Enable();
  shader.SetIntv("textures", tex_ids, 10);
  shader.Disable();
  // End of block

  Texture terrain("terrain", "assets/gfx/sprites/terrain.png");
  Texture tina("tina", "assets/gfx/sprites/tina.png");
  Texture guanaco("guanaco", "assets/gfx/sprites/guanaco-anim.png");

  for (int i = 0; i < 10; i++)
    for (int j = 0; j < 13; j++)
    {
      Sprite *sprite;
      sprite = new Sprite(j*64, i*64, 64, 64, terrain, rand()%25);

      layer.Add(sprite);
    }
  Sprite *Tina = new Sprite(400, 450, 46, 102, tina, 0);
  std::vector<unsigned int> idleanim = { 0, 1, 2, 3, 4, 5 };
  std::vector<unsigned int> walkanim = { 8, 9, 10, 11, 12, 13, 14, 15 };
  Tina->Animations.Add("idle", idleanim, 10);
  Tina->Animations.Add("walk", walkanim, 8);
  Tina->Animations.Play("walk");

  Sprite* Player = new Sprite(400 - 23, 300 - 51, 160, 120, guanaco, 0);
  std::vector<unsigned int> guanim = { 0, 1, 2, 3, 4 };
  Player->Animations.Add("run", guanim, 5);
  Player->Animations.Play("run");

  layer.Add(Tina);
  layer.Add(Player);

  SoundPlayer mplayer(&argc, argv);
  mplayer.Add("assets/audio/st-dawn_pollen.ogg");
  mplayer.Play();
  
  while (display.IsRunning())
  {
    display.Clear();
    layer.Render();
    Tina->Pos.x = 400 - 28 + 200*cosf(display.GetTime());
    Player->Update();
    Tina->Update();

    mplayer.Update();
    display.Update();
    if (Keys[GLFW_KEY_LEFT])
    {
      if (Player->Pos.x >= 0)
        Player->Pos.x -= 7;
    }
    if (Keys[GLFW_KEY_RIGHT])
    {
      if (Player->Pos.x + Player->GetWidth() <= display.GetWidth())
        Player->Pos.x += 7;
    }
  }

  return 0;
}

