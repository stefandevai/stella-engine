#pragma once

#include "animator.hpp"
#include "renderable.hpp"
#include "spritesheet.hpp"

namespace stella
{
namespace graphics
{
  class Texture;

  struct VertexData
  {
    glm::vec3 vertex;
    glm::vec2 uv;
    GLfloat tid;
    unsigned int color;
  };

  class Sprite : public Renderable
  {
  public:
    Animator Animations;

    Sprite (GLint x, GLint y, Texture& texture, const std::vector<GLuint>& frames);
    Sprite (GLint x, GLint y, Texture& texture, GLuint frame = 0);
    Sprite (GLint x, GLint y, GLint w, GLint h, Texture& texture, GLuint frame = 0);
    Sprite (glm::vec3 position, glm::vec2 frame_dimensions, Texture& texture, GLuint frame = 0);
    Sprite (glm::vec3 position, Texture& texture, GLuint frame = 0);
    Sprite (const glm::vec3 position, const glm::vec2 dimensions, const glm::vec2 offset, Texture& texture);
    Sprite (const Sprite& copied_sprite);
    Sprite& operator= (const Sprite& sprite);
    ~Sprite();

    bool operator< (const Sprite& other) const { return (this->Pos.z < other.Pos.z); }

    void Update();

    // Getters
    inline unsigned int GetColor() const { return Color; }
    inline GLuint GetCurrentFrame() const { return Frame; }
    inline SpriteSheet GetSpriteSheet() const { return *Sprites; }
    inline glm::vec2 GetFrameCoords() const { return Sprites->GetUV (this->Frame); }
    inline GLuint GetTexID() const;
    Texture* GetTexture() const;

    // Setters
    void SetFrame (const GLuint frame);
    void SetDirectFrame (const GLuint frame);
    void SetDirectColor (const unsigned color);
    void SetColorRGB (const unsigned r, const unsigned g, const unsigned b);
    void SetColorRGBA (const unsigned r, const unsigned int g, const unsigned b, const unsigned a);
    void SetAlpha (const unsigned a);
    void Flip() { this->SetScale (glm::vec2 (-1.f, 0.f)); }

  private:
    unsigned int Color;
    std::vector<GLuint> Frames;
    GLuint Frame = 0, NumberOfFrames;
    SpriteSheet* Sprites;
    Texture& Tex;
  };
} // namespace graphics
} // namespace stella
