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

      void Add(const char *filepath);
      void AddStream(std::string name, const char *filepath);
      void Play();
      void PlayStream(std::string name, bool loop = false);
      void Update();
      void initOgg(std::string filepath);

    private:
      std::vector<Sound*> Sounds;
      std::map<std::string, OggStream*> Streams;
  };
} }
