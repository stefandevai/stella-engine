#pragma once

namespace stella
{
namespace components
{
  struct TilePosition
  {
    TilePosition (const int x, const int y) : x (x), y (y) {}
    int x, y;
  };
} // namespace components
} // namespace stella
