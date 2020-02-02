#pragma once

#include <glm/glm.hpp>

namespace stella
{
namespace component
{
  struct Scroll
  {
    Scroll (glm::vec2 speed) : speed (speed) {}
    glm::vec2 speed;
  };
} // namespace component
} // namespace stella
