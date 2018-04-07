#pragma once

struct PlayerComponent {
  PlayerComponent() {}
  bool HasTorch = true;
  bool InAir = false;
  float JumpForce     = 200.f,
        Acceleration  = 400.f,
        Drag          = 500.f;

  std::bitset<2> SpriteDirection = std::bitset<2>(0x2);
};

