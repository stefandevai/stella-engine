#pragma once

#include "component.hpp"

namespace stella
{
namespace component
{
  struct Dimension : public Component
  {
    Dimension (const float w = 0.f, const float h = 0.f, const float d = 0.f)
      : Component ("Dimensions"), w (w), h (h), d (d), last_w (w), last_h (h)
    {
    }
    float w = 0.f, h = 0.f, d = 0.0f;
    float last_w = 0.f, last_h = 0.f;

    bool has_changed() const { return ((last_w != w) || (last_h != h)); }
  };
} // namespace component
} // namespace stella
