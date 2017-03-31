#pragma once

#ifdef __APPLE__
#include <OpenAL/al.h>
#else
#include <AL/al.h>
#endif

#define AUDIO_BUFFER_SIZE (4096 * 8)

namespace stella { namespace audio {
  class Playable
  {
    public:
      inline virtual ~Playable() {  }
      inline virtual void Play(const bool &loop = false) = 0;
      inline virtual void Pause(const bool &fadeOut = false) = 0;
      inline virtual void Stop(const bool &fadeOut = false) = 0;
      inline virtual void Update() = 0;

      inline virtual bool IsInitialized() = 0;

    protected:
      ALuint Source;
      ALenum Format;
      ALint State;
      bool Loop;

      inline Playable() {  }
  };
} }
