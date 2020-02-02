#pragma once

#include <glm/glm.hpp>

namespace stella
{
namespace components
{
  struct Movement
  {
    Movement (glm::vec2 target_velocity, bool gravity = true, bool constant_velocity = false)
      : TargetVelocity (target_velocity), Gravity (gravity), ConstantVelocity (constant_velocity)
    {
    }
    glm::vec2 TargetVelocity;
    bool Gravity, ConstantVelocity;
  };
} // namespace components
} // namespace stella
