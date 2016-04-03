#pragma once

#include <vector>

#ifdef __APPLE__
#include<OpenAL/al.h>
#else
#include <AL/al.h>
#endif

#define SOUND_BUFFER_SIZE (4096 * 8)

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
      ALuint ID;
      ALuint BufferID;
      ALuint SourceID;
      ALenum Format;
      ALsizei Freq;
      std::vector<char> BufferData;

      bool Loop;

      void init(const char* filepath);
      void loadOGG(const char* filepath, std::vector<char> &buffer, ALenum &format, ALsizei &freq);
  };
} }

