#pragma once

#include "texture.h"
#include "../../glm/glm/glm.hpp"

namespace stella { namespace graphics {
  class SpriteSheet
  {
    public:
      SpriteSheet(const Texture &texture, unsigned int framex, unsigned int framey, unsigned int number_of_frames = 0);
      ~SpriteSheet();

      glm::vec2 GetUV(GLuint frame);
      inline const GLuint GetWidth() const { return Frames.GetWidth(); }
      inline const GLuint GetHeight() const { return Frames.GetHeight(); }

      inline void SetOffset(GLuint x, GLuint y) { OffsetX = x; OffsetY = y; }
    private:
      const Texture &Frames;
      GLuint FrameX, FrameY, OffsetX, OffsetY, NumOfFrames, SizeInFramesX, SizeInFramesY;
  };
} }

