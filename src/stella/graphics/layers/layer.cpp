#include "stella/graphics/layers/layer.hpp"
#include "stella/graphics/opengl.hpp" // IWYU pragma: export

#include <algorithm>
#include <glm/gtc/matrix_transform.hpp> // IWYU pragma: export

namespace stella
{
namespace graphics
{
  Layer::Layer (std::shared_ptr<Renderer> renderer, bool fixed)
    : Ren (renderer), ViewMatrix (glm::mat4()), Fixed (fixed)
  {
  }

  Layer::~Layer() { this->Shad->Disable(); }

  void Layer::SetViewMatrix (glm::mat4 view) { this->ViewMatrix = view; }
} // namespace graphics
} // namespace stella
