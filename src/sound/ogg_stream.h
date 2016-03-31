#pragma once

#include <string>

#include <iostream>

#include <AL/al.h>
#include <ogg/ogg.h>
#include <vorbis/codec.h>
#include <vorbis/vorbisfile.h>

#define AUDIO_BUFFERS 4
#define AUDIO_BUFFER_SIZE (4096 * 8)

namespace stella { namespace audio {
  class OggStream
  {
    public:
      OggStream(const char* filepath);
      ~OggStream();

      bool Play(const bool& loop);
      bool Update();
      bool IsPlaying();
      bool IsInitialized();

    private:
      const char *FilePath;
      bool Loop, StreamOpened, Reseted;
      FILE *OggFile;
      OggVorbis_File StreamData;
      vorbis_info *VorbisInfo;
      vorbis_comment *VorbisComment;
      ALuint Buffers[AUDIO_BUFFERS];
      ALuint Source;
      ALenum Format;

      void openFile(const char* filepath);
      void displayInfo();
      bool streamBuffer(ALuint buffer);
      void emptyQueue();
      void clean();
      void checkErrors();
      std::string errorToString(int code);
  };
} }

