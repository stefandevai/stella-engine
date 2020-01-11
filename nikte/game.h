#pragma once

#include <stella/stella.h>
#include <stella/systems.h>
#include <stella/core/tile_map.h>

#include "player.h"

class Game : public stella::core::Game
{
  private:
    Player m_player{m_registry, m_display};
    stella::core::TileMap m_tile_map{"scripts/demo/map.lua", m_registry};

  public:
    Game();
    ~Game();

  private:
    void update(const double dt) override;
    //entt::registry::entity_type create_player();
    //void update_player(const double dt);
};