#include "stella/graphics/sprite.hpp"
#include "stella/graphics/texture.hpp"

#include <iostream>

namespace stella
{
namespace graphics
{
  Sprite::Sprite (GLint x, GLint y, Texture& texture, const std::vector<GLuint>& frames)
    : Renderable (glm::vec3 (x, y, 0.f), glm::vec2 (texture.GetWidth(), texture.GetHeight())), Frames (frames),
      Tex (texture)
  {
    this->Color          = 4294967295; // r = 255, g = 255, b = 255, a = 255
    this->NumberOfFrames = this->Frames.size();
    this->Frame          = 0;

    this->Sprites = new SpriteSheet (texture, this->Dimensions.x, this->Dimensions.y);
  }

  Sprite::Sprite (GLint x, GLint y, Texture& texture, GLuint frame)
    : Renderable (glm::vec3 (x, y, 0.f), glm::vec2 (texture.GetWidth(), texture.GetHeight())), Tex (texture)
  {
    this->Color          = 4294967295; // r = 255, g = 255, b = 255, a = 255
    this->NumberOfFrames = 1;
    this->Frame          = frame;

    this->Sprites = new SpriteSheet (texture, this->Dimensions.x, this->Dimensions.y);
  }

  Sprite::Sprite (GLint x, GLint y, GLint w, GLint h, Texture& texture, GLuint frame)
    : Renderable (x, y, w, h), Tex (texture)
  {
    this->Color          = 4294967295; // r = 255, g = 255, b = 255, a = 255
    this->NumberOfFrames = 1;
    this->Frame          = frame;

    this->Sprites = new SpriteSheet (texture, this->Dimensions.x, this->Dimensions.y);
  }

  Sprite::Sprite (glm::vec3 position, Texture& texture, GLuint frame)
    : Renderable (position, glm::vec2 (texture.GetWidth(), texture.GetHeight())), Tex (texture)
  {
    this->Color          = 4294967295; // r = 255, g = 255, b = 255, a = 255
    this->NumberOfFrames = 1;
    this->Frame          = frame;

    this->Sprites = new SpriteSheet (texture, this->Dimensions.x, this->Dimensions.y);
  }

  Sprite::Sprite (glm::vec3 position, glm::vec2 frame_dimensions, Texture& texture, GLuint frame)
    : Renderable (position, frame_dimensions), Tex (texture)
  {
    this->Color          = 4294967295; // r = 255, g = 255, b = 255, a = 255
    this->NumberOfFrames = 1;
    this->Frame          = frame;

    this->Sprites = new SpriteSheet (texture, this->Dimensions.x, this->Dimensions.y);
  }

  Sprite::Sprite (const glm::vec3 position, const glm::vec2 dimensions, const glm::vec2 offset, Texture& texture)
    : Renderable (position, dimensions), Tex (texture)
  {
    this->Color          = 4294967295; // r = 255, g = 255, b = 255, a = 255
    this->NumberOfFrames = 1;
    this->Frame          = 0;

    this->Sprites = new SpriteSheet (texture, this->Dimensions.x, this->Dimensions.y, 1);
    this->Sprites->SetUVOffset (offset.x, offset.y);
  }

  Sprite::Sprite (const Sprite& copied_sprite)
    : Renderable (copied_sprite.Pos.x,
                  copied_sprite.Pos.y,
                  copied_sprite.Dimensions.x,
                  copied_sprite.Dimensions.y,
                  copied_sprite.Visible),
      Tex (copied_sprite.Tex), Animations (copied_sprite.Animations), Frames (copied_sprite.Frames)
  {
    Color          = copied_sprite.Color;
    Frame          = copied_sprite.Frame;
    NumberOfFrames = copied_sprite.NumberOfFrames;
    Sprites        = new SpriteSheet (Tex, Dimensions.x, Dimensions.y);
  }
  Sprite& Sprite::operator= (const Sprite& sprite)
  {
    if (this != &sprite)
    {
      Animations     = sprite.Animations;
      Color          = sprite.Color;
      Frames         = sprite.Frames;
      Frame          = sprite.Frame;
      NumberOfFrames = sprite.NumberOfFrames;
      Sprites        = new SpriteSheet (Tex, Dimensions.x, Dimensions.y);
    }
    return *this;
  }

  Sprite::~Sprite() { delete Sprites; }

  void Sprite::Update() { this->Frame = Animations.Update(); }

  void Sprite::SetFrame (const GLuint frame) { Frame = frame % this->NumberOfFrames; }

  void Sprite::SetDirectFrame (const GLuint frame) { Frame = frame; }

  void Sprite::SetDirectColor (const unsigned color) { this->Color = color; }

  void Sprite::SetColorRGB (const unsigned r, const unsigned g, const unsigned b)
  {
    this->Color = 255 << 24 | b << 16 | g << 8 | r;
  }
  void Sprite::SetColorRGBA (const unsigned r, const unsigned g, const unsigned b, const unsigned a)
  {
    this->Color = a << 24 | b << 16 | g << 8 | r;
  }
  void Sprite::SetAlpha (unsigned a) { this->Color = a << 24; }

  Texture* Sprite::GetTexture() const { return &Tex; }
  GLuint Sprite::GetTexID() const { return Tex.GetID(); }
} // namespace graphics
} // namespace stella
