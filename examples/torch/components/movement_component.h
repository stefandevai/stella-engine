#pragma once

#include <glm/glm.hpp>

struct MovementComponent {
  MovementComponent(glm::vec2 acc, glm::vec2 target_velocity)
      : Acceleration(acc), TargetVelocity(target_velocity) {
  }
  glm::vec2 Acceleration, TargetVelocity;
};
