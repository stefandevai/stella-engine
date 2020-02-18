#pragma once

#include <stella/stella.h>
#include <stella/systems.h>
#include <stella/core/tile_map.h>
#include <stella/perlin.h>


#ifdef STELLA_BUILD_EDITOR
  #include <editor/editor_gui.h>
  #include <editor/widgets/map_editor.h>
#endif

#include "player.h"

namespace nikte
{
class Game : public stella::core::Game
{
private:
  Player m_player{m_registry, m_display};
  stella::core::TileMap m_tile_map{"scripts/demo/church_map.xml", m_registry};
  // stella::core::TileMap m_tile_map{"scripts/demo/map.lua", m_registry};
  stella::PerlinNoise m_perlin_generator{47};
  std::default_random_engine m_generator;
  std::uniform_real_distribution<double> m_perlin_distribution{0.0,1.0};
  std::uniform_int_distribution<int> m_flower_frame_distribution{0,3};

#ifdef STELLA_BUILD_EDITOR
  friend class stella::editor::EditorGui;
  friend class stella::widget::MapEditor;
#endif

public:
  Game();
  ~Game();

private:
  void update (const double dt) override;
  void m_load_flowers();
  void m_load_flower(const int x, const int y);
  // entt::registry::entity_type create_player();
  // void update_player(const double dt);
};
} // namespace nikte