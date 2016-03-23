#pragma once

namespace stella { namespace audio {
  class Sound
  {
    public:
      Sound(const char* filepath);
      ~Sound();

      void Play(bool fadeIn = false);
      void Pause(bool fadeOut = false);
      void Stop(bool fadeOut = false);

  };
} }

