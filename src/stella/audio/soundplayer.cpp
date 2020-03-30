#include "stella/audio/soundplayer.hpp"

#include <AL/alut.h>
#include <iostream>

namespace stella
{
namespace audio
{
  SoundPlayer::SoundPlayer() { alutInit (nullptr, nullptr); }

  SoundPlayer::~SoundPlayer()
  {
    for (auto i = this->Playables.begin(); i != this->Playables.end(); ++i)
      delete i->second;

    alutExit();
  }

  void SoundPlayer::AddSound (std::string name, const char* filepath)
  {
    Sound* sound = new Sound (filepath);
    this->Playables.insert (std::pair<std::string, Sound*> (name, sound));
  }

  void SoundPlayer::AddStream (std::string name, const char* filepath)
  {
    OggStream* stream = new OggStream (filepath);
    this->Playables.insert (std::pair<std::string, OggStream*> (name, stream));
  }

  void SoundPlayer::Play (std::string name, bool loop)
  {
    auto entry = this->Playables.find (name);
    if (entry == this->Playables.end())
      std::cout << "Couldn't find playable '" << name << "'" << std::endl;
    else
      entry->second->Play (loop);
  }

  void SoundPlayer::Stop (std::string name)
  {
    auto entry = this->Playables.find (name);
    if (entry == this->Playables.end())
      std::cout << "Couldn't find playable '" << name << "'" << std::endl;
    else
      entry->second->Stop();
  }

  void SoundPlayer::Update()
  {
    for (auto i = this->Playables.begin(); i != this->Playables.end(); ++i)
    {
      if (i->second->IsInitialized())
        i->second->Update();
    }
  }
} // namespace audio
} // namespace stella
