#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <stella/stella.h>
#include <vector>

#include "scenelayer.h"

int main(int argc, char *argv[]) {
  using namespace stella;
  using namespace graphics;
  using namespace audio;

	std::array<bool, 1024> Keys;
	Keys.fill(false);

  Display display(800, 600, "Stella", Keys);
  display.SetClearColor(22, 38, 47);

  // TODO Move this block to init on singleton class
  GLint tex_ids[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};

  Shader shader("assets/shaders/basic_shader.vsh",
                "assets/shaders/light_shader.fsh");
  glm::mat4 proj = glm::ortho(0.0f, (GLfloat)display.GetWidth(),
                              (GLfloat)display.GetHeight(), 0.0f, -1.0f, 1.0f);

  SceneLayer layer(&shader, proj);

  shader.Enable();
  shader.SetIntv("textures", tex_ids, 10);
  shader.Disable();
  // End of block

  Texture terrain("terrain", "assets/sprites/terrain.png");
  Texture tina("tina", "assets/sprites/tina.png");
  Texture guanaco("guanaco", "assets/sprites/guanaco-anim.png");

  for (int i = 0; i < 10; i++)
    for (int j = 0; j < 13; j++) {
      Sprite *sprite;
      sprite = new Sprite(j * 64, i * 64, 64, 64, terrain, rand() % 29);

      layer.Add(sprite);
    }
  Sprite *Tina = new Sprite(400 - 23, 300 - 51, 46, 102, tina, 0);
  std::vector<unsigned int> idleanim = {0, 1, 2, 3, 4, 5};
  std::vector<unsigned int> walkanim = {8, 9, 10, 11, 12, 13, 14, 15};
  Tina->Animations.Add("idle", idleanim, 10);
  Tina->Animations.Add("walk", walkanim, 8);
  Tina->Animations.Play("walk");

  Sprite *Player = new Sprite(400 - 80, 450, 160, 120, guanaco, 0);
  std::vector<unsigned int> guanim = {0, 1, 2, 3, 4};
  Player->Animations.Add("run", guanim, 5);
  Player->Animations.Play("run");

  layer.Add(Tina);
  layer.Add(Player);

  SoundPlayer mplayer(&argc, argv);
  mplayer.AddStream("dawn-pollen", "assets/audio/st-dawn_pollen.ogg");
  mplayer.Play("dawn-pollen", true);
  mplayer.AddSound("star", "assets/audio/star.ogg");

  bool idle = false, spacepressed = false;
  int anim_counter = 0, light_counter = 0;

  while (display.IsRunning()) {
    display.Clear();
    layer.Render();
    Player->Update();
    if (!idle)
      Tina->Pos.x = 400 - 28 + 200 * cosf(anim_counter++ / 50.0f);
    Tina->Update();

    shader.Enable();
    shader.SetVec2f("lightPos", Tina->Pos.x + Tina->GetWidth() / 2,
                    Tina->Pos.y + Tina->GetHeight() / 2);
    shader.SetFloat("lightCounter", (GLfloat)light_counter);
    shader.SetFloat("lightIntensity", (GLfloat)0.7f);
    ++light_counter;

    mplayer.Update();
    display.Update();
    if (Keys[GLFW_KEY_LEFT] || Keys[GLFW_KEY_A]) {
      if (Player->Pos.x >= 0)
        Player->Pos.x -= 7;
    }
    if (Keys[GLFW_KEY_RIGHT] || Keys[GLFW_KEY_D]) {
      if (Player->Pos.x + Player->GetWidth() <= display.GetWidth())
        Player->Pos.x += 7;
    }
    if (Keys[GLFW_KEY_UP] || Keys[GLFW_KEY_W]) {
      if (Player->Pos.y >= 0)
        Player->Pos.y -= 7;
    }
    if (Keys[GLFW_KEY_DOWN] || Keys[GLFW_KEY_S]) {
      if (Player->Pos.y + Player->GetHeight() <= display.GetHeight())
        Player->Pos.y += 7;
    }

    if (Keys[GLFW_KEY_SPACE] && !spacepressed) {
      if (!idle) {
        idle = true;
        Tina->Animations.Play("idle");
      } else if (idle) {
        idle = false;
        Tina->Animations.Play("walk");
      }
      spacepressed = true;
    } else if (!Keys[GLFW_KEY_SPACE] && spacepressed)
      spacepressed = false;

    if (Keys[GLFW_KEY_P])
      mplayer.Play("star");
  }

  shader.Disable();
  return 0;
}
