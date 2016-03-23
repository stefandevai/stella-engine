#include <iostream>
#include <vector>
#include <cmath>
#include "Dependencies/glm/glm/glm.hpp"
#include "Dependencies/glm/glm/gtc/matrix_transform.hpp"
#include "src/stella.h"

#include <AL/al.h>
#include <AL/alut.h>
#include <vorbis/vorbisfile.h>

#define AL_BUF_SIZE 32768

int main(int argc, char *argv[])
{
  using namespace stella;
  using namespace graphics;

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

  ALint state;
  ALuint bufferID;
  ALuint sourceID;
  ALenum format;
  ALsizei freq;

  std::vector<char> bufferData;

  alutInit(&argc, argv);
  alGenBuffers(1, &bufferID);
  alGenSources(1, &sourceID);
  alListener3f(AL_POSITION, 0.0f, 0.0f, 0.0f);
  alSource3f(sourceID, AL_POSITION, 0.0f, 0.0f, 0.0f);

  int endian = 0;
  int bitStream;
  long bytes;
  char array[AL_BUF_SIZE];
  FILE *file;

  file = fopen("assets/audio/tune1.ogg", "rb");
  vorbis_info *pInfo;
  OggVorbis_File oggFile;
  ov_open(file, &oggFile, NULL, 0);
  pInfo = ov_info(&oggFile, -1);
  if (pInfo->channels == 1)
    format = AL_FORMAT_MONO16;
  else
    format = AL_FORMAT_STEREO16;
  freq = pInfo->rate;

  do {
    bytes = ov_read(&oggFile, array, AL_BUF_SIZE, endian, 2, 1, &bitStream);
    bufferData.insert(bufferData.end(), array, array+bytes);
  } while (bytes > 0);

  ov_clear(&oggFile);

  alBufferData(bufferID, format, &bufferData[0], static_cast<ALsizei>(bufferData.size()), freq);
  alSourcei(sourceID, AL_BUFFER, bufferID);
  alSourcePlay(sourceID);

  while (display.IsRunning())
  {
    display.Clear();
    layer.Render();
    layer2.Render();
    Stella->Pos.x = 400 - 28 + 200*cosf(display.GetTime());

    alGetSourcei(sourceID, AL_SOURCE_STATE, &state);
    display.Update();
  }
  alDeleteBuffers(1, &bufferID);
  alDeleteSources(1, &sourceID);
  alutExit();

  shader.Disable();

  return 0;
}

