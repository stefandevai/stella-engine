#pragma once

namespace stella
{
namespace component
{
  struct Fog
  {
    Fog (float elevation, bool flat = false) : elevation (elevation), flat (flat) {}
    float elevation;
    bool flat;
  };
} // namespace component
} // namespace stella
