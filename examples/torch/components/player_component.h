#pragma once

struct PlayerComponent {
  PlayerComponent() {}
  bool HasTorch = true;
  bool Jumping = false;
  const float JumpForce     = 200.f,
              Acceleration  = 400.f,
              Drag          = 500.f;

};

