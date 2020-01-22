#pragma once

#include "./imgui/imgui.h"
#include "stella/graphics/texture.h"

namespace stella
{
namespace editor
{
  class TilesetEditor
  {
    private:
        std::string m_path;
        graphics::Texture m_texture;
        int m_tile_dimensions[2] = {32,32};
        int m_texture_w = 0, m_texture_h = 0;
        
        bool m_tile_collidable = false;
        bool m_tile_scriptable = false;
        int m_selected_tile_id = 0;
        ImTextureID m_selected_tile_texture = nullptr;
        ImVec2 m_selected_tile_size;
        ImVec2 m_selected_tile_uv1;
        ImVec2 m_selected_tile_uv2;

        bool has_changed_id = false;

    public:
      TilesetEditor(const std::string& path);
      ~TilesetEditor();
      void render();
      ImVec2 pos2tile(const int x, const int y);
      ImVec2 get_tile_dimensions();
      void render_tile_sprite(const ImVec2& pos, const float alpha = 1.f);

      inline int get_selected_tile_id()
      {
          return m_selected_tile_id;
      }

      inline bool get_selected_tile_collidable()
      {
          return m_tile_collidable;
      }

      inline bool get_selected_tile_scriptable()
      {
          return m_tile_scriptable;
      }
  };
}
}

