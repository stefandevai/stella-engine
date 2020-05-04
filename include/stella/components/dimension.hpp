#pragma once

#include "component.hpp"

namespace stella
{
namespace component
{
  struct Dimension : public Component
  {
    Dimension() : Component ("Dimension") {}
    Dimension (const float w, const float h) : Component ("Dimensions"), w (w), h (h), last_w (w), last_h (h) {}
    float w = 0.f, h = 0.f;
    float last_w = 0.f, last_h = 0.f;

    bool has_changed() const { return ((last_w != w) || (last_h != h)); }
  };
} // namespace component
} // namespace stella
