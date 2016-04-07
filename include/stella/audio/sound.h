#pragma once

#include "playable.h"

#include <vector>

namespace stella { namespace audio {
  class Sound : public Playable
  {
    public:
      Sound(const char* filepath);
      ~Sound();

      void Play(const bool &loop = false) override;
      void Pause(const bool &fadeOut = false) override;
      void Stop(const bool &fadeOut = false) override;
      void Update() override;

      bool IsInitialized() override;

    private:
      ALuint Buffer;
      ALsizei Freq;
      std::vector<char> BufferData;
      bool Loaded;

      void init(const char* filepath);
      void loadOGG(const char* filepath, std::vector<char> &buffer, ALenum &format, ALsizei &freq);
  };
} }

