#pragma once

#include <vector>

#include <AL/al.h>

#define SOUND_BUFFER_SIZE 32768

namespace stella { namespace audio {
  class Sound
  {
    public:
      Sound(const char* filepath);
      ~Sound();

      void Play(bool fadeIn = false);
      void Pause(bool fadeOut = false);
      void Stop(bool fadeOut = false);
      void Update();

    private:
      ALint State;
      ALuint BufferID;
      ALuint SourceID;
      ALenum Format;
      ALsizei Freq;
      std::vector<char> BufferData;

      void init(const char* filepath);
      void loadOGG(const char* filepath, std::vector<char> &buffer, ALenum &format, ALsizei &freq);
  };
} }

