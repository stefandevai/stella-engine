#pragma once

#include <string>

#include "playable.h"

#include "vorbisogg.h"

#define STREAM_BUFFERS 4

namespace stella
{
namespace audio
{
  class OggStream : public Playable
  {
  public:
    explicit OggStream (const char* filepath);
    ~OggStream();

    void Play (const bool& loop) override;
    void Pause (const bool& fadeOut = false) override;
    void Resume (const bool& fadeOut = false) override;
    void Stop (const bool& fadeOut = false) override;
    void Update() override;
    bool IsPlaying();
    bool IsStoped();
    bool IsPaused();
    // virtual bool IsInitialized() const override;
    inline bool IsInitialized() override { return this->StreamOpened; }

  private:
    const char* FilePath;
    bool StreamOpened, Reseted;
    // FILE *OggFile;
    OggVorbis_File StreamData;
    vorbis_info* VorbisInfo       = nullptr;
    vorbis_comment* VorbisComment = nullptr;
    ALuint Buffers[STREAM_BUFFERS];

    void openFile (const char* filepath);
    // const void displayInfo() const;
    bool streamBuffer (ALuint buffer);
    void emptyQueue();
    void clean();
  };
} // namespace audio
} // namespace stella
