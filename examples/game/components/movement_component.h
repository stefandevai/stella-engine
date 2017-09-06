#pragma once

struct Vector2D { float x, y; };

struct MovementComponent
{
  MovementComponent(float acc, float drag, float maxVelocity, bool grav = true) : Acceleration(acc), Drag(drag), MaxVelocity(maxVelocity), Gravity(grav) {}
  float Acceleration, Drag, MaxVelocity;
  bool Gravity;
  Vector2D Vel, Acc;
  inline void stopX() { Vel.x = 0.0f; }
  inline void stopY() { Vel.y = 0.0f; }
  inline void accel(float x, float y) { Acc.x = x; Acc.y = y; }
};

