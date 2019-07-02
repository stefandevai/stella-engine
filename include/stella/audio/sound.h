#pragma once

#include "playable.h"

#include <vector>

namespace stella {
namespace audio {
class Sound : public Playable {
public:
  explicit Sound(const char *filepath);
  ~Sound();

  void Play(const bool &loop = false) override;
  void Pause(const bool &fadeOut = false) override;
  void Resume(const bool &fadeOut = false) override;
  void Stop(const bool &fadeOut = false) override;
  void Update() override;
  //virtual bool IsInitialized() override;
  inline bool IsInitialized() override { return this->Loaded; }

private:
  ALuint Buffer;
  ALsizei Freq;
  std::vector<char> BufferData;
  bool Loaded;

  void init(const char *filepath);
};
} // namespace audio
} // namespace stella
