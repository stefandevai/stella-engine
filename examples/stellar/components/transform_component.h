#pragma once

#include <glm/glm.hpp>

struct TransformComponent {
  TransformComponent(float rotation, glm::vec2 scale) : Rotation(rotation), Scale(scale) {}

  float Rotation;
  glm::vec2 Scale;
};

