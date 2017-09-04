#pragma once

struct Vector2D { float x, y; };

struct MovementComponent
{
  MovementComponent(float acc, float drag, float maxVelocity) : Acceleration(acc), Drag(drag), MaxVelocity(maxVelocity) {}
  float Acceleration, Drag, MaxVelocity;
  Vector2D Vel, Acc;
};

