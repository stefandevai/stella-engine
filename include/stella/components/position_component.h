#pragma once

namespace stella
{
namespace components
{
  struct PositionComponent
  {
    PositionComponent (float x, float y, float z = 0.f) : x (x), y (y), z (z) {}
    float x, y, z;
  };
} // namespace components
} // namespace stella
