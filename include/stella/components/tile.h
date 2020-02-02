#pragma once

namespace stella
{
namespace components
{
  struct Tile
  {
    Tile (const unsigned layer_id, const bool collidable) : layer_id (layer_id), collidable (collidable) {}
    unsigned layer_id = 0;
    bool collidable;
  };
} // namespace components
} // namespace stella
