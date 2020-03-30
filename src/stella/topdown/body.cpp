#include "stella/topdown/body.hpp"

namespace stella
{
namespace topdown
{
  Body::Body (glm::vec2 position, glm::vec2 dimension, const double movement_speed)
    : Position (position), Dimension (dimension), m_movement_speed (movement_speed)
  {
    this->Target.x      = this->Position.x;
    this->Target.y      = this->Position.y;
    this->MovementDelay = 1.0f / (m_movement_speed / MAX_MOVEMENT_SPEED);
  }

  Body::~Body() {}
} // namespace topdown
} // namespace stella
