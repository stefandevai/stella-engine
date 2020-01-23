#pragma once

#include "./imgui/imgui.h"
#include "stella/core/tile_map.h"

namespace nikte
{
  class Game;
}

namespace stella
{
namespace editor
{
  class MapEditor
  {
    private:
      core::TileMap& m_tile_map;
      std::shared_ptr<core::MapGrid> m_selected_layer = nullptr;
      char m_map_name[128];
      char m_path[128];
      int m_map_size[2] = {0,0};

      static const int min_map_size = 1;
      static const int max_map_size = 1000;

    public:
      MapEditor(nikte::Game& game);
      ~MapEditor();
      void render();
      void reset_map_settings();
      void update_map_settings();
      inline int get_selected_layer_id() const
      {
        if (m_selected_layer)
        {
          return m_selected_layer->get_id();
        }
        return -1;
      }

      inline const std::string get_map_name() const { return std::string(m_map_name); }
      inline const std::string get_map_path() const { return std::string(m_path); }
  };
}
}

