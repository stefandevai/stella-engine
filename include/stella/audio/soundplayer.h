#pragma once

#include <map>
#include <string>
#include <vector>

#include "ogg_stream.h"
#include "sound.h"

namespace stella
{
namespace audio
{
  class SoundPlayer
  {
  public:
    SoundPlayer();
    ~SoundPlayer();

    void AddSound (std::string name, const char* filepath);
    void AddStream (std::string name, const char* filepath);
    void Play (std::string name, bool loop = false);
    void Stop (std::string name);
    void Update();

  private:
    std::map<std::string, Playable*> Playables;
  };
} // namespace audio
} // namespace stella
