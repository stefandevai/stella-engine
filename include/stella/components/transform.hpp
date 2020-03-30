#pragma once

#include <glm/glm.hpp>

namespace stella
{
namespace component
{
  struct Transform
  {
    Transform (const double rotation = 0.0f, const glm::vec2 scale = glm::vec2 (1.f, 1.f))
      : Rotation (rotation), Scale (scale)
    {
    }

    double Rotation;
    glm::vec2 Scale;
    bool ResizedX = false, ResizedY = false;
  };
} // namespace component
} // namespace stella
