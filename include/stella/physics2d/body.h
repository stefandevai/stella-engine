#pragma once

#include <glm/glm.hpp>

namespace stella {
namespace physics2d {
class Body {
  public:
    Body(glm::vec2 position, glm::vec2 dimension);
    ~Body();

    inline const glm::vec2 GetPosition() const { return Position; }

    glm::vec2 Position, Dimension, Velocity = glm::vec2(0.f, 0.f), TargetVelocity = glm::vec2(0.f, 0.f), Acceleration = glm::vec2(0.f, 0.f), Drag = glm::vec2(0.f, 0.f);
    bool Gravity = true;
};
}}

