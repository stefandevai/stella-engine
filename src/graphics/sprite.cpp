#include "sprite.h"

namespace stella { namespace graphics { 
  Sprite::Sprite(GLint x, GLint y, std::vector<GLuint> frames)
    : Frames(frames)
  {
    this->Pos = glm::vec2(static_cast<GLfloat>(x), static_cast<GLfloat>(y));
    this->NumberOfFrames = this->Frames.size();
    this->Frame = 0;
  }

  Sprite::~Sprite()
  {

  }
  
  void Sprite::SetFrame(GLuint frame) 
  { 
    Frame = frame%this->NumberOfFrames; 
  }
}}

