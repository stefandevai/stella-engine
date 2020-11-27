#include "stella/graphics/renderable.hpp"

#include "stella/graphics/opengl.hpp"
#include <vector>

namespace stella
{
namespace graphics
{
  Renderable::Renderable (glm::vec3 position, glm::vec2 dimensions, GLboolean visible) : Pos (position), Dimensions (dimensions), RealDimensions (dimensions), Visible (visible) {}
  Renderable::Renderable (GLint x, GLint y, GLint w, GLint h, GLboolean visible) : Renderable (glm::vec3 (x, y, 0.f), glm::vec2 (w, h), visible) {}
  // Renderable::Renderable (GLint x, GLint y)
  //   : Renderable (glm::vec3 (x, y, 0.f), glm::vec2 (texture.GetWidth(), texture.GetHeight()), texture)
  // {
  // }
  // Renderable::Renderable (glm::vec3 position, Texture& texture)
  //   : Renderable (position, glm::vec2 (texture.GetWidth(), texture.GetHeight()), texture)
  // {
  // }
  Renderable::~Renderable() {}
  // const std::string& Renderable::GetTexName() const { return Tex.GetName(); }

} // namespace graphics
} // namespace stella
