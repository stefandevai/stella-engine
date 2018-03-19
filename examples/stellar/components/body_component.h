#pragma once

#include <bitset>

struct BodyComponent {
  BodyComponent(int width, int height, int offsetx, int offsety, bool isStatic)
      : Width(width), Height(height), OffsetX(offsetx), OffsetY(offsety),
        Static(isStatic) {}
  int Width, Height, OffsetX, OffsetY;
  int Velocity = 0;
  bool Colliding = false;
  bool Static;
  std::bitset<4> ColDir;
};
