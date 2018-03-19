#pragma once

struct MovementComponent {
  int Direction, NewDirection, Velocity;
  bool Eased;
  static bool Finished;

  inline MovementComponent(int dir, int vel, bool eased = true)
      : Direction(dir), Velocity(vel), Eased(eased) {
    NewDirection = dir;
  }
};
