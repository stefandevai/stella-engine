#pragma once

struct BodyComponent
{
  BodyComponent(float width, float height, float offsetx, float offsety, bool isStatic) : Width(width), Height(height), OffsetX(offsetx), OffsetY(offsety), Static(isStatic){}
  float Width, Height, OffsetX, OffsetY;
  bool Static;
};

