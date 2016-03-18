#include "sprite.h"

#include <iostream>

namespace stella { namespace graphics { 
  Sprite::Sprite(GLint x, GLint y, Texture &texture, const std::vector<GLuint> &frames)
    : Tex(texture), Frames(frames)
  {
    this->Pos = glm::vec2((GLfloat)x, (GLfloat)y);
    this->Dimensions = glm::vec2(this->Tex.GetWidth(), this->Tex.GetHeight());
    this->Color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    this->Visible = GL_TRUE;
    this->NumberOfFrames = this->Frames.size();
    this->Frame = 0;
  }
  
  Sprite::Sprite(GLint x, GLint y, Texture &texture, GLuint frame)
    : Tex(texture)
  {
    this->Pos = glm::vec2((GLfloat)x, (GLfloat)y);
    this->Dimensions = glm::vec2(this->Tex.GetWidth(), this->Tex.GetHeight());
    this->Color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    this->Visible = GL_TRUE;
    this->NumberOfFrames = 1;
    this->Frame = frame;

    this->Sprites = new SpriteSheet(texture, this->Dimensions.x, this->Dimensions.y);
  }

  Sprite::Sprite(GLint x, GLint y, GLint w, GLint h, Texture &texture, GLuint frame)
    : Tex(texture)
  {
    this->Pos = glm::vec2((GLfloat)x, (GLfloat)y);
    this->Dimensions = glm::vec2((GLfloat)w, (GLfloat)h);
    this->Color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    this->Visible = GL_TRUE;
    this->NumberOfFrames = 1;
    this->Frame = frame;

    this->Sprites = new SpriteSheet(texture, this->Dimensions.x, this->Dimensions.y);
  }
  
  Sprite::~Sprite()
  {
    delete Sprites;
  }
  
  void Sprite::SetFrame(GLuint frame) 
  { 
    Frame = frame%this->NumberOfFrames; 
  }
}}

