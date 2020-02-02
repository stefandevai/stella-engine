#pragma once

#include <stella/stella.h>
#include <stella/systems.h>
#include <stella/core/tile_map.h>

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
    stella::core::TileMap m_tile_map{"scripts/demo/map.lua", m_registry};

    #ifdef STELLA_BUILD_EDITOR
      friend class stella::editor::EditorGui;
      friend class stella::widget::MapEditor;
    #endif

  public:
    Game();
    ~Game();

  private:
    void update(const double dt) override;
    //entt::registry::entity_type create_player();
    //void update_player(const double dt);
};
}