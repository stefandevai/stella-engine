#include "soundplayer.h"

#include <AL/alut.h>
#include <iostream>

namespace stella { namespace audio {
  SoundPlayer::SoundPlayer(int *argcp, char **argv)
  {
    alutInit(argcp, argv);
  }

  SoundPlayer::~SoundPlayer()
  {
    for (auto i : Sounds)
      delete i;

    for (auto i = this->Streams.begin(); i != this->Streams.end(); ++i)
      delete i->second;

    alutExit();
  }

  void SoundPlayer::Add(const char *filepath)
  {
    Sound *sound = new Sound(filepath);
    this->Sounds.push_back(sound);
  }
  
  void SoundPlayer::AddStream(std::string name, const char *filepath)
  {
    OggStream *stream = new OggStream(filepath);
    this->Streams.insert(std::pair<std::string, OggStream*>(name, stream));
  }

  void SoundPlayer::Play()
  {
    for (auto i : Sounds)
      i->Play();
  }
  
  void SoundPlayer::PlayStream(std::string name, bool loop)
  {
    auto entry = this->Streams.find(name);
    if (entry == this->Streams.end())
      std::cout << "Couldn't find stream '" << name << "'" << std::endl;
    else
      entry->second->Play(loop);
  }
  
  void SoundPlayer::Update()
  {
    //for (auto i : Sounds)
    //  i->Update();
    /*Stream.update();
    if (!Stream.playing())
    {
      if (!Stream.playback())
        std::cout << std::string("Ogg stopped") << std::endl;
      else
        std::cout << "Ogg interrupted" << std::endl;
    }*/
  
    for (auto i = this->Streams.begin(); i != this->Streams.end(); ++i)
    {
      if (i->second->IsInitialized())
        i->second->Update();
    }
  }
} }
