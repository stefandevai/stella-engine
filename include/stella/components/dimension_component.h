#pragma once

namespace stella
{
namespace components
{
  struct DimensionComponent
  {
    DimensionComponent (float w, float h) : w (w), h (h) {}
    float w, h;
  };
} // namespace components
} // namespace stella
