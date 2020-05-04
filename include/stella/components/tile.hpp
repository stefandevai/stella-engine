#pragma once

#include "component.hpp"

namespace stella
{
namespace component
{
  struct Tile : public Component
  {
    Tile (const unsigned layer_id, const bool collidable) : Component ("Tile"), layer_id (layer_id), collidable (collidable) {}
    unsigned layer_id = 0;
    bool collidable;
  };
} // namespace component
} // namespace stella
