#pragma once

#include "component.hpp"

namespace stella
{
namespace component
{
  struct Position : public Component
  {
    Position() : Component ("Position") {}
    Position (const float x, const float y, const float z = 0.f) : Component ("Position"), x (x), y (y), z (z), last_x (x), last_y (y), last_z (z) {}
    float x = 0.f, y = 0.f, z = 0.f;
    float last_x = 0.f, last_y = 0.f, last_z = 0.f;

    bool has_changed() const { return ((last_x != x) || (last_y != y) || (last_z != z)); }
  };
} // namespace component
} // namespace stella
