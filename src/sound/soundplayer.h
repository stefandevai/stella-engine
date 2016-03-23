#pragma once

#include <vector>

#include "sound.h"

namespace stella { namespace audio {
  class SoundPlayer
  {
    public:
      SoundPlayer(int *argcp, char **argv);
      ~SoundPlayer();

      void Add(const char *filepath);
      void Play();
      void Update();

    private:
      std::vector<Sound*> Sounds;
  };
} }
