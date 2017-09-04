#pragma once

struct Vector2D { float x, y; };

struct MovementComponent
{
  MovementComponent(float acc, float drag, float maxVelocity, bool grav = true) : Acceleration(acc), Drag(drag), MaxVelocity(maxVelocity), Gravity(grav) {}
  float Acceleration, Drag, MaxVelocity;
  bool Gravity;
  Vector2D Vel, Acc;
};

