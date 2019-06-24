#pragma once

namespace stella {
namespace components {
struct PlayerComponent {
  PlayerComponent() {}
  bool HasTorch = true;
  bool InAir = false;
  float JumpForce     = 150.f,
        Acceleration  = 500.f,
        Drag          = 500.f;

  std::bitset<2> SpriteDirection = std::bitset<2>(0x2);
};
} // namespace components
} // namespace stella

