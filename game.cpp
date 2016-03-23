#include <iostream>
#include <vector>
#include <map>
#include <cmath>
#include "Dependencies/glm/glm/glm.hpp"
#include "Dependencies/glm/glm/gtc/matrix_transform.hpp"
#include "src/stella.h"

#include <ft2build.h>
#include FT_FREETYPE_H

struct Character {
  GLuint TextureID;
  glm::ivec2 Size;
  glm::ivec2 Bearing;
  GLuint Advance;
};

std::map<GLchar, Character> Characters;
void RenderText(stella::graphics::Shader &shader, std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color);

int main(int argc, char *argv[])
{
  using namespace stella;
  using namespace graphics;
  using namespace audio;

  Display display(800, 600, "Stella");
  display.SetClearColor(22, 38, 47);

  // TODO Move this block to init on singleton class
  GLint tex_ids[] =
  {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11
  };

  Shader shader("assets/shaders/basic_shader.vsh", "assets/shaders/basic_shader.fsh");
  glm::mat4 proj = glm::ortho(0.0f, (GLfloat)display.GetWidth(), (GLfloat)display.GetHeight(), 0.0f, -1.0f, 1.0f);

  SceneLayer layer(&shader, proj);
  SceneLayer layer2(&shader, proj);

  shader.Enable();
  shader.SetIntv("textures", tex_ids, 10);
  shader.Disable();
  // End of block

  Texture guanaco("guanaco", "assets/gfx/sprites/guanaco.png");
  Texture stella("stella", "assets/gfx/sprites/stella.png");
  Texture terrain("terrain", "assets/gfx/sprites/terrain.png");

//  srand(47);
  for (int i = 0; i < 10; i++)
    for (int j = 0; j < 13; j++)
    {
      Sprite *sprite;
//      int rand_num = rand();
//      if (rand_num%2 == 0)
//        sprite = new Sprite(j*64, i*64, 64, 64, terrain, rand()%25);
//      else if (rand_num%3 == 0)
//        sprite = new Sprite(j*64, i*64, 28, 28, guanaco, 0);
//      else
//        sprite = new Sprite(j*64, i*64, 28, 28, stella, 0);
      sprite = new Sprite(j*64, i*64, 64, 64, terrain, 0);

      layer.Add(sprite);
    }
  Sprite *Stella = new Sprite(400, 100, 28, 28, stella, 0);
  layer2.Add(Stella);

  //SoundPlayer mplayer(&argc, argv);
  //mplayer.Add("assets/audio/tune1.ogg");
  //mplayer.Play();
  
  FT_Library ft;
  if (FT_Init_FreeType(&ft))
    std::cout << "FreeType Error" << std::endl;

  FT_Face face;
  if (FT_New_Face(ft, "assets/fonts/Raleway-Medium.ttf", 0, &face))
    std::cout << "Failed to load font" << std::endl;

  FT_Set_Pixel_Sizes(face, 0, 48);
  


  while (display.IsRunning())
  {
    display.Clear();
    layer.Render();
    layer2.Render();
    Stella->Pos.x = 400 - 28 + 200*cosf(display.GetTime());

    //mplayer.Update();
    display.Update();
  }

  shader.Disable();

  return 0;
}

