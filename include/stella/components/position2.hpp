#pragma once

#include "component.hpp"

namespace stella::component
{
  struct Position2 : public Component
  {
    Position2 (const float x, const float y, const float z = 0.f)
      : Component ("Position2"), x (x), y (y), z (z)
    { }

    float x = 0.f;
    float y = 0.f;
    float z = 0.f;
  };
} // namespace stella

