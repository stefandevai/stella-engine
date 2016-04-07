#include "graphics/spritesheet.h"

#include <string>
#include <iostream>

namespace stella { namespace graphics {
  SpriteSheet::SpriteSheet(const Texture& texture, unsigned int framex, unsigned int framey, unsigned int number_of_frames)
    : Frames(texture), FrameX(framex), FrameY(framey), NumOfFrames(number_of_frames)
  {
    this->SizeInFramesX = Frames.GetWidth()/this->FrameX;
    this->SizeInFramesY = Frames.GetHeight()/this->FrameY;

    if (!number_of_frames)
    {
      this->NumOfFrames = this->SizeInFramesX * this->SizeInFramesY;
    }
  }

  SpriteSheet::~SpriteSheet()
  {
  }

  glm::vec2 SpriteSheet::GetUV(GLuint frame)
  {
    if (this->NumOfFrames <= 1)
    {
      return glm::vec2(0.0f, 1.0f);
    }
    frame += 1;
    GLfloat w = (GLfloat)Frames.GetWidth();
    GLfloat h = (GLfloat)Frames.GetHeight();

    if (frame > this->NumOfFrames)
    {
      std::cout << "SpriteSheet::Warning: frame has a greater value than the number of frames.\nModulus will be applied to select another frame." << std::endl;
      frame = frame%this->NumOfFrames;
    }

    GLint coordX = frame%this->SizeInFramesX - 1;
    if (coordX == -1) 
    { 
      coordX = this->SizeInFramesX - 1;
    }
    GLuint coordY = ceil(frame/(GLfloat)this->SizeInFramesX) - 1;

    GLfloat uvX = coordX*this->FrameX/w;
    GLfloat uvY = 1.0f - coordY*this->FrameY/h;

    return glm::vec2(uvX, uvY); 
  }

} }

