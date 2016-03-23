#include "sound.h"

#include <vorbis/vorbisfile.h>
#include <iostream>

namespace stella { namespace audio {
  Sound::Sound(const char *filepath)
  {
    this->init(filepath);
  }
  Sound::~Sound()
  {
    alDeleteBuffers(1, &this->BufferID);
    alDeleteSources(1, &this->SourceID);
  }

  void Sound::Play(bool fadeIn)
  {
    alSourcePlay(this->SourceID);
  }
  
  void Sound::Update()
  {
    alGetSourcei(this->SourceID, AL_SOURCE_STATE, &this->State);
  }

  void Sound::init(const char *filepath)
  {
    alGenBuffers(1, &this->BufferID);
    alGenSources(1, &this->SourceID);
    alListener3f(AL_POSITION, 0.0f, 0.0f, 0.0f);
    alSource3f(this->SourceID, AL_POSITION, 0.0f, 0.0f, 0.0f);
    this->loadOGG(filepath, this->BufferData, this->Format, this->Freq);
    alBufferData(this->BufferID, this->Format, &this->BufferData[0], static_cast<ALsizei>(this->BufferData.size()), this->Freq);
    alSourcei(this->SourceID, AL_BUFFER, this->BufferID);
  }
  
  void Sound::loadOGG(const char* filepath, std::vector<char> &buffer, ALenum &format, ALsizei &freq)
  {
    int endian = 0;
    int bitStream;
    long bytes;
    char array[SOUND_BUFFER_SIZE];
    FILE *file;

    file = fopen(filepath, "rb");
    vorbis_info *pInfo;
    OggVorbis_File oggFile;
    ov_open(file, &oggFile, NULL, 0);
    pInfo = ov_info(&oggFile, -1);
    if (pInfo->channels == 1)
      format = AL_FORMAT_MONO16;
    else
      format = AL_FORMAT_STEREO16;
    freq = pInfo->rate;

    do {
      bytes = ov_read(&oggFile, array, SOUND_BUFFER_SIZE, endian, 2, 1, &bitStream);
      buffer.insert(buffer.end(), array, array+bytes);
    } while (bytes > 0);

    ov_clear(&oggFile);
  }
} }

