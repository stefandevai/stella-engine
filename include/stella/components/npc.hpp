#pragma once

#include "component.hpp"

#include <string>
#include "tile_position.hpp"

namespace stella
{
namespace component
{
  enum NpcState
  {
    None,
    Walking,
    Talking
  };
  struct NPC : public Component
  {
    NPC() : Component ("NPC") {}
    NPC (std::string script_path) : Component ("NPC"), script_path (script_path) {}
    std::string script_path{};
    NpcState state = None;
    TilePosition origin{0, 0};
    int walk_radius = 5;
  };
} // namespace component
} // namespace stella
