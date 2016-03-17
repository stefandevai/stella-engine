#include "sprite.h"

#include <iostream>

namespace stella { namespace graphics { 
  Sprite::Sprite(GLint x, GLint y, const Texture &texture, const std::vector<GLuint> &frames)
    : SpriteSheet(texture), Frames(frames)
  {
    this->Pos = glm::vec2((GLfloat)x, (GLfloat)y);
    this->Dimensions = glm::vec2(SpriteSheet.GetWidth(), SpriteSheet.GetHeight());
    this->Color = glm::vec4(51/255.0f, 242/255.0f, 140/255.0f, 1.0f);
    this->Visible = GL_TRUE;
    this->NumberOfFrames = this->Frames.size();
    this->Frame = 0;
  }

  Sprite::Sprite(GLint x, GLint y, const Texture &texture, GLuint frame)
    : SpriteSheet(texture)
  {
    this->Pos = glm::vec2((GLfloat)x, (GLfloat)y);
    this->Dimensions = glm::vec2(this->SpriteSheet.GetWidth(), this->SpriteSheet.GetHeight());
    this->Color = glm::vec4(64/255.0f, 173/255.0f, 115/255.0f, 1.0f);
    this->Visible = GL_TRUE;
    this->NumberOfFrames = 1;
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

