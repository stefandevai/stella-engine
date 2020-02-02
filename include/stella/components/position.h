#pragma once

namespace stella
{
namespace component
{
  struct Position
  {
    Position (const float x, const float y, const float z = 0.f) : x (x), y (y), z (z), last_x (x), last_y (y), last_z (z) {}
    float x, y, z;
    float last_x, last_y, last_z;

    const bool has_changed() const
    {
      return ((last_x != x) || (last_y != y) || (last_z != z));
    }
  };
} // namespace component
} // namespace stella
