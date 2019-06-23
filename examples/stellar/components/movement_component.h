#pragma once

struct Vector2D {
  float x, y;
};

struct MovementComponent {
  MovementComponent(float acc, float drag, float maxVelocity, bool grav = true)
      : Acceleration(acc), Drag(drag), MaxVelocity(maxVelocity), Gravity(grav) {
    Acc.x = acc;
    Acc.y = 0.0f;
  }
  float Acceleration, Drag, MaxVelocity;
  bool Gravity;
  Vector2D Vel = {0.0f, 0.0f}, Acc = {0.0f, 0.0f};
  inline void stopX() {
    Acc.x = 0.0f;
    Vel.x = 0.0f;
  }
  inline void stopY() {
    Gravity = false;
    Acc.y = 0.0f;
    Vel.y = 0.0f;
  }
  inline void accelX(float sign) { Acc.x += this->Acceleration * sign; }
  inline void accelY(float sign) { Acc.y += this->Acceleration * sign; }
  inline void desaccelX() { Acc.x = 0.0f; }
  inline void desaccelY() { Acc.y = 0.0f; }
};
