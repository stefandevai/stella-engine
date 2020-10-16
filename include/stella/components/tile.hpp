#pragma once

#include "component.hpp"

namespace stella
{
namespace component
{
  struct Tile : public Component
  {
    Tile() : Component ("Tile") {}
    Tile (const unsigned layer_id, const bool collidable)
      : Component ("Tile"), layer_id (layer_id), collidable (collidable)
    {
    }
    int layer_id    = -1;
    bool collidable = false;
  };
} // namespace component
} // namespace stella
