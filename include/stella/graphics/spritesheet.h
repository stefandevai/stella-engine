#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <iostream>

namespace stella {
namespace graphics {
class Texture;

class SpriteSheet {
public:
  SpriteSheet(const Texture &texture, unsigned int framex, unsigned int framey,
              unsigned int number_of_frames = 0);
  ~SpriteSheet();

  glm::vec2 GetUV(unsigned int frame);
  unsigned int GetWidth() const; 
  unsigned int GetHeight() const;

  inline void SetPixelOffset(const unsigned x, const unsigned y) {
    this->PixelOffsetX = x;
    this->PixelOffsetY = y;
  }

  inline void SetUVOffset(const float x, const float y) {
    this->UVOffsetX = x;
    this->UVOffsetY = y;
  }
float UVOffsetX = 0.f, UVOffsetY = 0.f;
private:
  const Texture &Frames;
  unsigned int FrameX, FrameY, PixelOffsetX = 0, PixelOffsetY = 0, NumOfFrames, SizeInFramesX,
      SizeInFramesY;
  
};
} // namespace graphics
} // namespace stella
