#include "stella/graphics/renderable.h"
#include "stella/graphics/texture.h"

#include "stella/graphics/opengl.h"
#include <vector>

namespace stella
{
namespace graphics
{
  Renderable::Renderable (glm::vec3 position, glm::vec2 dimensions, Texture& texture, GLboolean visible)
    : Pos (position), Dimensions (dimensions), RealDimensions (dimensions), Tex (texture), Visible (visible)
  {
  }
  Renderable::Renderable (GLint x, GLint y, GLint w, GLint h, Texture& texture, GLboolean visible)
    : Renderable (glm::vec3 (x, y, 0.f), glm::vec2 (w, h), texture, visible)
  {
  }
  Renderable::Renderable (GLint x, GLint y, Texture& texture)
    : Renderable (glm::vec3 (x, y, 0.f), glm::vec2 (texture.GetWidth(), texture.GetHeight()), texture)
  {
  }
  Renderable::Renderable (glm::vec3 position, Texture& texture)
    : Renderable (position, glm::vec2 (texture.GetWidth(), texture.GetHeight()), texture)
  {
  }
  Renderable::~Renderable() {}

  GLuint Renderable::GetTexID() const { return Tex.GetID(); }
  // const std::string& Renderable::GetTexName() const { return Tex.GetName(); }
  Texture* Renderable::GetTexture() const { return &Tex; }

} // namespace graphics
} // namespace stella
