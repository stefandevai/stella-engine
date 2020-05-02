#pragma once

namespace stella
{
namespace component
{
  struct Fog
  {
    Fog (float h, bool flat = false) : h (h), flat (flat) {}
    float h;
    bool flat;
  };
} // namespace component
} // namespace stella