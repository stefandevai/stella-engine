#pragma once

namespace stella
{
namespace component
{
  struct Tile
  {
    Tile (const unsigned layer_id, const bool collidable) : layer_id (layer_id), collidable (collidable) {}
    unsigned layer_id = 0;
    bool collidable;
  };
} // namespace component
} // namespace stella
