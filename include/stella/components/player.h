#pragma once

#include <bitset>

namespace stella
{
namespace components
{
  struct Player
  {
    Player() {}
    bool InAir      = false;
    float JumpForce = 250.f, Acceleration = 4400.f, Drag = 2200.f;

    std::bitset<2> SpriteDirection = std::bitset<2> (0x2);
  };
} // namespace components
} // namespace stella
