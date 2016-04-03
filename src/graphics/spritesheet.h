#pragma once

#include "texture.h"
#include "../../dependencies/glm/glm/glm.hpp"

#include <vector>

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

      void push(const glm::mat4& mat);
      void pop();
    private:
      const Texture &Frames;
      GLuint FrameX, FrameY, OffsetX, OffsetY, NumOfFrames, SizeInFramesX, SizeInFramesY;
      std::vector<glm::vec4> TransfStack;
      const glm::mat4 *TransfBack;
  };
} }

