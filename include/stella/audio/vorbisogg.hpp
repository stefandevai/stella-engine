// This is a wrapper header for GCC in order to ignore vorbis ogg warnings
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#include <ogg/ogg.h>           // IWYU pragma: export
#include <vorbis/codec.h>      // IWYU pragma: export
#include <vorbis/vorbisfile.h> // IWYU pragma: export
#pragma GCC diagnostic pop
