#pragma once

struct MovementComponent
{
  int Direction, Velocity;

  MovementComponent(int dir, int vel) : Direction(dir), Velocity(vel) {}
};

