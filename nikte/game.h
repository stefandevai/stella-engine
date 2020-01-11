#pragma once

#include <stella/stella.h>
//#include <stella/systems.h>
//#include <stella/core/tile_map.h>
//#include <stella/graphics/layers/water_layer.h>
//#include <stella/physics2d/water_surface.h>

#include "player.h"

class Game : public stella::core::Game
{
  private:
    //Player m_player{m_registry, m_display, m_sound_player};
    //entt::registry::entity_type m_fps_text = m_registry.create();
    //entt::registry::entity_type m_ms_text = m_registry.create();
    //std::shared_ptr<stella::core::TileMap> m_tile_map = nullptr;
    //stella::core::TileMap m_tile_map{"scripts/level1/test_map.lua", m_registry};

    /*stella::physics2d::GridWorld test_world{m_tile_map};
    std::shared_ptr<stella::physics2d::WaterSurface> water_surface = std::make_shared<stella::physics2d::WaterSurface>(896.0);
    stella::graphics::WaterLayer water_layer{(float)m_display.GetWidth(), (float)m_display.GetHeight(), true};
    std::vector<std::shared_ptr<stella::graphics::Shape>> water_shapes;*/

  public:
    Game();
    ~Game();

  private:
    void update(const double dt) override;
    //entt::registry::entity_type create_player();
    //void update_player(const double dt);
};