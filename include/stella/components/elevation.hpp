#pragma once

namespace stella
{
namespace component
{
  struct Elevation
  {
    Elevation (float h, bool flat) : h (h), flat (flat) {}
    float h;
    bool flat;
  };
} // namespace component
} // namespace stella
