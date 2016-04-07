#pragma once

#include <vector>
#include <map>
#include <string>

#include "sound.h"
#include "ogg_stream.h"

namespace stella { namespace audio {
  class SoundPlayer
  {
    public:
      SoundPlayer(int *argcp, char **argv);
      ~SoundPlayer();

      void AddSound(std::string name, const char *filepath);
      void AddStream(std::string name, const char *filepath);
      void Play(std::string name, bool loop = false);
      void Update();

    private:
      std::map<std::string, Playable*> Playables;
  };
} }
