#pragma once

namespace stella
{
namespace components
{
  struct DimensionComponent
  {
    DimensionComponent (float w, float h) : w (w), h (h), last_w (w), last_h (h) {}
    float w, h;
    float last_w, last_h;

    const bool has_changed() const
    {
      return ((last_w != w) || (last_h != h));
    }
  };
} // namespace components
} // namespace stella
