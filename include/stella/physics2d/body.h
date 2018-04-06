#pragma once

#include <memory>
#include <bitset>
#include <glm/glm.hpp>

namespace stella {
namespace physics2d {
class Body {
  public:
    Body(glm::vec2 position, glm::vec2 dimension);
    ~Body();

    inline glm::vec2 GetPosition() const { return Position; }
    inline bool CollidingTop() const { return this->Collisions.test(0); }
    inline bool CollidingRight() const { return this->Collisions.test(1); }
    inline bool CollidingBottom() const { return this->Collisions.test(2); }
    inline bool CollidingLeft() const { return this->Collisions.test(3); }

    glm::vec2 Position, Dimension, LastPosition = glm::vec2(0.f, 0.f), Velocity = glm::vec2(0.f, 0.f), TargetVelocity = glm::vec2(0.f, 0.f), Acceleration = glm::vec2(0.f, 0.f), Drag = glm::vec2(0.f, 0.f), CollisionVector = glm::vec2(0.f, 0.f);
    bool IsStatic = false, Gravity = false;
    std::bitset<4> Collisions;
};
}}

