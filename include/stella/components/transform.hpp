#pragma once

#include <glm/glm.hpp>

namespace stella
{
namespace component
{
  struct Transform
  {
    Transform (const glm::vec3& rotation = glm::vec3 (0.f, 0.f, 0.f), const glm::vec3& scale = glm::vec3 (1.f, 1.f, 1.f))
      : rotation (rotation), scale (scale)
    {
    }

    glm::vec3 rotation;
    glm::vec3 scale;
  };
} // namespace component
} // namespace stella
