#pragma once

#include <glm/glm.hpp>

struct TransformComponent {
  TransformComponent(float rotation, glm::vec2 scale) : Rotation(rotation), Scale(scale) {}

  float Rotation = 0.f;
  glm::vec2 Scale = glm::vec2(1.f, 1.f);
  bool ResizedX = false, ResizedY = false;
};

