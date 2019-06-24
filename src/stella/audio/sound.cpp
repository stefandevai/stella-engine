#include "stella/audio/sound.h"

#include <vorbis/vorbisfile.h>

namespace {
void loadOGG(const char *filepath, std::vector<char> &buffer,
                    ALenum &format, ALsizei &freq) {
  int endian = 0;
  int bitStream;
  long bytes;
  char array[AUDIO_BUFFER_SIZE];
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
    bytes =
        ov_read(&oggFile, array, AUDIO_BUFFER_SIZE, endian, 2, 1, &bitStream);
    buffer.insert(buffer.end(), array, array + bytes);
  } while (bytes > 0);

  ov_clear(&oggFile);
}
}

namespace stella {
namespace audio {
Sound::Sound(const char *filepath) : Playable() {
  this->Loaded = false;
  this->init(filepath);
}

Sound::~Sound() {
  alDeleteBuffers(1, &this->Buffer);
  alDeleteSources(1, &this->Source);
}

void Sound::Play(const bool &loop) { alSourcePlay(this->Source); }

void Sound::Pause(const bool &fadeOut) {}

void Sound::Stop(const bool &fadeOut) {}

void Sound::Update() {
  alGetSourcei(this->Source, AL_SOURCE_STATE, &this->State);
}

//bool Sound::IsInitialized() { return this->Loaded; }

void Sound::init(const char *filepath) {
  alGenBuffers(1, &this->Buffer);
  alGenSources(1, &this->Source);
  alListener3f(AL_POSITION, 0.0f, 0.0f, 0.0f);
  alSource3f(this->Source, AL_POSITION, 0.0f, 0.0f, 0.0f);
  loadOGG(filepath, this->BufferData, this->Format, this->Freq);
  alBufferData(this->Buffer, this->Format, &this->BufferData[0],
               static_cast<ALsizei>(this->BufferData.size()), this->Freq);
  alSourcei(this->Source, AL_BUFFER, this->Buffer);
  this->Loaded = true;
}
} // namespace audio
} // namespace stella
