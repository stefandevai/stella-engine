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
    alutExit();
  }

  void SoundPlayer::Add(const char *filepath)
  {
    Sound *sound = new Sound(filepath);
    this->Sounds.push_back(sound);
  }

  void SoundPlayer::Play()
  {
    for (auto i : Sounds)
      i->Play();
  }
  
  void SoundPlayer::Update()
  {
    for (auto i : Sounds)
      i->Update();
  }
} }
