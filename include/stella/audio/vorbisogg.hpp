// This is a wrapper header for GCC in order to ignore vorbis ogg warnings
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#include <ogg/ogg.h>
#include <vorbis/codec.h>
#include <vorbis/vorbisfile.h>
#pragma GCC diagnostic pop
