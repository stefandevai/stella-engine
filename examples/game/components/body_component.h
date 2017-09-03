#pragma once

struct BodyComponent
{
  BodyComponent(int width, int height, int offsetx, int offsety, bool isStatic) : Width(width), Height(height), OffsetX(offsetx), OffsetY(offsety), Static(isStatic){}
  int Width, Height, OffsetX, OffsetY;
  bool Colliding = false;
  bool Static;
};

