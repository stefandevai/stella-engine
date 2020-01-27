#include "stella/physics2d/body.h"

namespace stella
{
namespace physics2d
{
  Body::Body (glm::vec2 position, glm::vec2 dimension, glm::vec2 drag, bool collide_with_borders)
    : Position (position), Dimension (dimension), Drag (drag), CollideWithBorders (collide_with_borders)
  {
  }

  Body::~Body() {}
} // namespace physics2d
} // namespace stella
