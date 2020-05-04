#pragma once

#include "component.hpp"

namespace stella
{
namespace component
{
  struct Dimension : public Component
  {
    Dimension (const float w, const float h) : Component ("Dimensions"), w (w), h (h), last_w (w), last_h (h) {}
    float w, h;
    float last_w, last_h;

    bool has_changed() const { return ((last_w != w) || (last_h != h)); }
  };
} // namespace component
} // namespace stella
