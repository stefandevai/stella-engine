#pragma once

#include <string>

#include "playable.h"

#include <ogg/ogg.h>
#include <vorbis/codec.h>
#include <vorbis/vorbisfile.h>

#define STREAM_BUFFERS 4

namespace stella {
namespace audio {
class OggStream : public Playable {
public:
  OggStream(const char *filepath);
  ~OggStream();

  void Play(const bool &loop);
  void Pause(const bool &fadeOut = false);
  void Stop(const bool &fadeOut = false);
  void Update();

  bool IsPlaying();
  bool IsInitialized();

private:
  const char *FilePath;
  bool StreamOpened, Reseted;
  FILE *OggFile;
  OggVorbis_File StreamData;
  vorbis_info *VorbisInfo;
  vorbis_comment *VorbisComment;
  ALuint Buffers[STREAM_BUFFERS];

  void openFile(const char *filepath);
  void displayInfo();
  bool streamBuffer(ALuint buffer);
  void emptyQueue();
  void clean();
  void checkErrors();
  std::string errorToString(int code);
};
} // namespace audio
} // namespace stella
