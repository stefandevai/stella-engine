#pragma once

#include <bitset>
#include <glm/glm.hpp>

namespace stella
{
namespace topdown
{
  enum BodyState
  {
    NONE,
    IDLE,
    MOVING
  };
  enum BodyDirection
  {
    TOP    = 1 << 0,
    RIGHT  = 1 << 1,
    BOTTOM = 1 << 2,
    LEFT   = 1 << 3
  };

  class Body
  {
  public:
    Body (const glm::vec2 position, const glm::vec2 dimension, const double movement_speed);
    ~Body();

    inline glm::vec2 GetPosition() const { return Position; }
    inline bool CollidingTop() const { return this->Collisions.test (0); }
    inline bool CollidingRight() const { return this->Collisions.test (1); }
    inline bool CollidingBottom() const { return this->Collisions.test (2); }
    inline bool CollidingLeft() const { return this->Collisions.test (3); }

    inline bool MovingTop() const { return this->Movement.test (0); }
    inline bool MovingRight() const { return this->Movement.test (1); }
    inline bool MovingBottom() const { return this->Movement.test (2); }
    inline bool MovingLeft() const { return this->Movement.test (3); }
    inline bool MovingDiagonal() const
    {
      return ((this->Movement.test (0) || this->Movement.test (2)) &&
              (this->Movement.test (1) || this->Movement.test (3)));
    }
    inline bool WillMove() const
    {
      return (this->Movement.test (0) || this->Movement.test (1) || this->Movement.test (2) || this->Movement.test (3));
    }

    inline void ResetMovement() { this->Movement.reset(); }
    inline void MoveTop() { this->Movement.set (0); }
    inline void MoveRight() { this->Movement.set (1); }
    inline void MoveBottom() { this->Movement.set (2); }
    inline void MoveLeft() { this->Movement.set (3); }

    glm::vec2 Position, Dimension, Target, LastPosition = glm::vec2 (0.f, 0.f), Velocity = glm::vec2 (0.f, 0.f),
                                           TargetVelocity = glm::vec2 (0.f, 0.f), Acceleration = glm::vec2 (0.f, 0.f),
                                           Drag = glm::vec2 (0.f, 0.f), CollisionVector = glm::vec2 (0.f, 0.f);
    bool IsStatic = false, CollideWithBorders;
    // Top, right, bottom, left collisions
    std::bitset<4> Collisions;
    float Transition = 0.f, LastTransition = 0.f, MovementDelay = 20.f;

    BodyState state = BodyState::IDLE;
    int direction   = BodyDirection::BOTTOM;

  private:
    std::bitset<4> Movement;
    double m_movement_speed;
    const double MAX_MOVEMENT_SPEED = 200.0;
  };
} // namespace topdown
} // namespace stella
