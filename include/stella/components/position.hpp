#pragma once

#include "component.hpp"

namespace stella
{
namespace component
{
  struct Position : public Component
  {
    Position (const float x, const float y, const float z = 0.f)
      : Component ("Position"), x (x), y (y), z (z), last_x (x), last_y (y), last_z (z)
    {
    }
    float x, y, z;
    float last_x, last_y, last_z;

    bool has_changed() const { return ((last_x != x) || (last_y != y) || (last_z != z)); }
  };
} // namespace component
} // namespace stella
