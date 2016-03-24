#include "sprite.h"

namespace stella { namespace graphics { 
  Sprite::Sprite(GLint x, GLint y, Texture &texture, const std::vector<GLuint> &frames)
    : Renderable(x, y, texture), Frames(frames)
  {
    this->Color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    this->NumberOfFrames = this->Frames.size();
    this->Frame = 0;

    this->Sprites = new SpriteSheet(texture, this->Dimensions.x, this->Dimensions.y);
  }
  
  Sprite::Sprite(GLint x, GLint y, Texture &texture, GLuint frame)
    : Renderable(x, y, texture)
  {
    this->Color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    this->NumberOfFrames = 1;
    this->Frame = frame;

    this->Sprites = new SpriteSheet(texture, this->Dimensions.x, this->Dimensions.y);
  }

  Sprite::Sprite(GLint x, GLint y, GLint w, GLint h, Texture &texture, GLuint frame)
    : Renderable(x, y, w, h, texture)
  {
    this->Color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
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

