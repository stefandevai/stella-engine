#pragma once

#include <glm/glm.hpp>

struct MovementComponent {
  MovementComponent(glm::vec2 target_velocity)
      : TargetVelocity(target_velocity) {
  }
  glm::vec2 Acceleration = glm::vec2(0.0f, 0.0f),
            Velocity     = glm::vec2(0.0f, 0.0f),
            Drag         = glm::vec2(500.f, 0.f),
            TargetVelocity;
  bool Gravity = true;
};
