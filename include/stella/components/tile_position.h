#pragma once

namespace stella
{
namespace component
{
  struct TilePosition
  {
    TilePosition (const int x, const int y) : x (x), y (y) {}
    int x, y;
  };
} // namespace component
} // namespace stella
