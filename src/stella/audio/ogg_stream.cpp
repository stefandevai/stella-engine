#include "stella/audio/ogg_stream.h"

#include <iostream>

namespace {
void checkErrors() {
  int error = alGetError();
  if (error != AL_NO_ERROR)
    std::cout << "OpenAL error." << std::endl;
}

std::string errorToString(int code) {
  switch (code) {
  case OV_EREAD:
    return "Read from media.";
  case OV_ENOTVORBIS:
    return "Not vorbis data.";
  case OV_EVERSION:
    return "Vorbis version mismatch";
  case OV_EBADHEADER:
    return "Invalid vorbis header.";
  case OV_EFAULT:
    return "Internal logic fault";
  default:
    return "Unknown Ogg error.";
  }
}
}

namespace stella {
namespace audio {
OggStream::OggStream(const char *filepath) : FilePath(filepath) {
  this->StreamOpened = false;
  this->Reseted = true;
}

OggStream::~OggStream() {
  if (this->StreamOpened) {
    this->clean();
  }
}

void OggStream::Play(const bool &loop) {
  if (IsPlaying())
    return;
  this->Loop = loop;
  if (!this->StreamOpened)
    this->openFile(this->FilePath);
  for (int i = 0; i < STREAM_BUFFERS; ++i)
    if (!streamBuffer(this->Buffers[i]))
      return;

  alSourceQueueBuffers(this->Source, STREAM_BUFFERS, this->Buffers);
  alSourcePlay(this->Source);
  this->Reseted = false;
}

void OggStream::Pause(const bool &fadeOut) {}

void OggStream::Stop(const bool &fadeOut) {}

void OggStream::Update() {
  ALenum state;
  alGetSourcei(this->Source, AL_SOURCE_STATE, &state);
  if (state == AL_PAUSED)
    return;

  int processed;
  //bool active = true;
  alGetSourcei(this->Source, AL_BUFFERS_PROCESSED, &processed);
  while (processed--) {
    ALuint buffer;
    alSourceUnqueueBuffers(this->Source, 1, &buffer);
    checkErrors();

    bool active = this->streamBuffer(buffer);
    if (active) {
      alSourceQueueBuffers(this->Source, 1, &buffer);
      checkErrors();
    }
  }

  if (state == AL_STOPPED) {
    if (this->Loop) {
      ov_raw_seek(&this->StreamData, 0);
      this->Play(this->Loop);
    } else if (!this->Reseted) {
      this->Reseted = true;
      ov_raw_seek(&this->StreamData, 0);
    }
    //active = this->Loop;
  }
}

//bool OggStream::IsInitialized() const { return this->StreamOpened; }

bool OggStream::IsPlaying() {
  if (!this->StreamOpened)
    return false;
  ALenum state;
  alGetSourcei(this->Source, AL_SOURCE_STATE, &state);
  return (state == AL_PLAYING);
}

void OggStream::openFile(const char *filepath) {
  int result;
  FILE *file;
  if (!(file = fopen(filepath, "rb")))
    std::cout << "Could not open ogg file." << std::endl;
  if ((result = ov_open(file, &this->StreamData, NULL, 0)) < 0) {
    fclose(file);
    std::cout << "Could not open ogg stream." << errorToString(result)
              << std::endl;
  }
  this->StreamOpened = true;
  this->VorbisInfo = ov_info(&this->StreamData, -1);
  this->VorbisComment = ov_comment(&this->StreamData, -1);
  if (this->VorbisInfo->channels == 1)
    this->Format = AL_FORMAT_MONO16;
  else
    this->Format = AL_FORMAT_STEREO16;

  alGenBuffers(STREAM_BUFFERS, this->Buffers);
  checkErrors();
  alGenSources(1, &this->Source);
  checkErrors();

  alSource3f(this->Source, AL_POSITION, 0.0, 0.0, 0.0);
  alSource3f(this->Source, AL_VELOCITY, 0.0, 0.0, 0.0);
  alSource3f(this->Source, AL_DIRECTION, 0.0, 0.0, 0.0);
  alSourcef(this->Source, AL_ROLLOFF_FACTOR, 0.0);
  alSourcei(this->Source, AL_SOURCE_RELATIVE, AL_TRUE);

  this->displayInfo();
}

const void OggStream::displayInfo() const {
  std::cout << "version: " << this->VorbisInfo->version
            << "channels: " << this->VorbisInfo->channels << "rate (hz) "
            << this->VorbisInfo->rate << std::endl;
  for (int i = 0; i < this->VorbisComment->comments; i++)
    std::cout << this->VorbisComment->user_comments[i] << std::endl;
}

bool OggStream::streamBuffer(ALuint buffer) {
  char data[AUDIO_BUFFER_SIZE];
  int size = 0;
  int section;
  while (size < AUDIO_BUFFER_SIZE) {
    const int result = ov_read(&this->StreamData, data + size, AUDIO_BUFFER_SIZE - size,
                     0, 2, 1, &section);
    if (result > 0)
      size += result;
    else if (result < 0)
      std::cout << errorToString(result) << std::endl;
    else
      break;
  }
  if (size == 0)
    return false;
  alBufferData(buffer, this->Format, data, size, this->VorbisInfo->rate);
  checkErrors();

  return true;
}

void OggStream::emptyQueue() {
  int queued;
  alGetSourcei(this->Source, AL_BUFFERS_QUEUED, &queued);

  while (queued--) {
    ALuint buffer;
    alSourceUnqueueBuffers(this->Source, 1, &buffer);
    checkErrors();
  }
}

void OggStream::clean() {
  this->StreamOpened = false;
  alSourceStop(this->Source);
  this->emptyQueue();
  alDeleteSources(1, &this->Source);
  checkErrors();
  alDeleteBuffers(STREAM_BUFFERS, this->Buffers);
  checkErrors();
  ov_clear(&this->StreamData);
}
} // namespace audio
} // namespace stella
