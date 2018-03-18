#pragma once

struct MovementComponent
{
  int Direction, NewDirection, Velocity;

  inline MovementComponent(int dir, int vel) : Direction(dir), Velocity(vel) { NewDirection = dir; }
};

