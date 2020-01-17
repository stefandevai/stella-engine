#include "stella/graphics/sprite.h"

#include <iostream>

namespace stella {
namespace graphics {
Sprite::Sprite(GLint x, GLint y, Texture &texture,
               const std::vector<GLuint> &frames)
    : Renderable(x, y, texture), Frames(frames) {
  this->Color = 4294967295; // r = 255, g = 255, b = 255, a = 255
  this->NumberOfFrames = this->Frames.size();
  this->Frame = 0;

  this->Sprites =
      new SpriteSheet(texture, this->Dimensions.x, this->Dimensions.y);
}

Sprite::Sprite(GLint x, GLint y, Texture &texture, GLuint frame)
    : Renderable(x, y, texture) {
  this->Color = 4294967295; // r = 255, g = 255, b = 255, a = 255
  this->NumberOfFrames = 1;
  this->Frame = frame;

  this->Sprites =
      new SpriteSheet(texture, this->Dimensions.x, this->Dimensions.y);
}

Sprite::Sprite(GLint x, GLint y, GLint w, GLint h, Texture &texture,
               GLuint frame)
    : Renderable(x, y, w, h, texture) {
  this->Color = 4294967295; // r = 255, g = 255, b = 255, a = 255
  this->NumberOfFrames = 1;
  this->Frame = frame;

  this->Sprites =
      new SpriteSheet(texture, this->Dimensions.x, this->Dimensions.y);
}

Sprite::Sprite(glm::vec3 position, Texture &texture, GLuint frame)
  : Renderable(position, texture)
{
  this->Color = 4294967295; // r = 255, g = 255, b = 255, a = 255
  this->NumberOfFrames = 1;
  this->Frame = frame;

  this->Sprites =
      new SpriteSheet(texture, this->Dimensions.x, this->Dimensions.y);

}

Sprite::Sprite(glm::vec3 position, glm::vec2 frame_dimensions, Texture &texture, GLuint frame)
  : Renderable(position, frame_dimensions, texture)
{
  this->Color = 4294967295; // r = 255, g = 255, b = 255, a = 255
  this->NumberOfFrames = 1;
  this->Frame = frame;

  this->Sprites =
      new SpriteSheet(texture, this->Dimensions.x, this->Dimensions.y);

}

Sprite::Sprite(const glm::vec3 position, const glm::vec2 dimensions, const glm::vec2 offset, Texture &texture)
  : Renderable(position, dimensions, texture)
{
  this->Color = 4294967295; // r = 255, g = 255, b = 255, a = 255
  this->NumberOfFrames = 1;
  this->Frame = 0;

  this->Sprites =
      new SpriteSheet(texture, this->Dimensions.x, this->Dimensions.y, 1);
  this->Sprites->SetUVOffset(offset.x, offset.y);
}

Sprite::Sprite(const Sprite &copied_sprite)
    : Renderable(copied_sprite.Pos.x, copied_sprite.Pos.y, copied_sprite.Dimensions.x, copied_sprite.Dimensions.y, copied_sprite.Tex, copied_sprite.Visible), Animations(copied_sprite.Animations), Frames(copied_sprite.Frames) {
  Color = copied_sprite.Color;
  Frame = copied_sprite.Frame;
  NumberOfFrames = copied_sprite.NumberOfFrames;
  Sprites = 
      new SpriteSheet(Tex, Dimensions.x, Dimensions.y);
}
Sprite& Sprite::operator=(const Sprite &sprite) {
  if (this != &sprite) {
    Animations = sprite.Animations;
    Color = sprite.Color;
    Frames = sprite.Frames;
    Frame = sprite.Frame;
    NumberOfFrames = sprite.NumberOfFrames;
    Sprites = 
        new SpriteSheet(Tex, Dimensions.x, Dimensions.y);
  }
  return *this;
}

Sprite::~Sprite() { delete Sprites; }

void Sprite::Update() { this->Frame = Animations.Update(); }

void Sprite::SetFrame(GLuint frame) { Frame = frame % this->NumberOfFrames; }

void Sprite::SetDirectFrame(GLuint frame) { Frame = frame; }

void Sprite::SetColorRGB(int r, int g, int b) {
	this->Color = 255 << 24 | b << 16 | g << 8 | r;
}
void Sprite::SetColorRGBA(unsigned int r, unsigned int g, unsigned int b, unsigned int a) {
	this->Color = a << 24 | b << 16 | g << 8 | r;
}
void Sprite::SetAlpha(int a) {
	this->Color = a << 24;
}
} // namespace graphics
} // namespace stella
