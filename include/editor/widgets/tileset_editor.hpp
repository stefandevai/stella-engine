#pragma once

#include "widget.hpp"
//#include "../../lib/imgui/imgui.hpp"
#include "stella/graphics/texture.hpp"
#include <entt/entity/entity.hpp>
#include <entt/entity/registry.hpp>

namespace editor
{
namespace widget
{
  class TilesetEditor : public Widget
  {
  private:
    std::string m_path;
    char m_script_path[128];

    int m_tile_dimensions[2] = {32, 32};
    int m_texture_w = 0, m_texture_h = 0;
    int m_z_coord = 0;

    bool m_tile_collidable              = false;
    bool m_tile_scriptable              = false;
    int m_selected_tile_id              = 0;
    ImTextureID m_selected_tile_texture = nullptr;
    ImVec2 m_selected_tile_size;
    ImVec2 m_selected_tile_uv1;
    ImVec2 m_selected_tile_uv2;

    bool has_changed_id = false;

  public:
    stella::graphics::Texture texture;

    TilesetEditor (const std::string& path);
    ~TilesetEditor();
    void render();
    ImVec2 pos2tile (const double x, const double y);
    void render_tile_sprite (const ImVec2& pos, const float alpha = 1.f);

    inline const ImVec2 get_tile_dimensions() const { return ImVec2 (m_tile_dimensions[0], m_tile_dimensions[1]); }
    inline const ImVec2 get_texture_dimensions_in_tiles() const { return ImVec2 (m_texture_w / m_tile_dimensions[0], m_texture_h / m_tile_dimensions[1]); }

    entt::entity get_entity (entt::registry& registry);
    inline int get_selected_tile_id() { return m_selected_tile_id; }

    inline bool get_selected_tile_collidable() { return m_tile_collidable; }

    inline bool get_selected_tile_scriptable() { return m_tile_scriptable; }
  };
} // namespace widget
} // namespace editor
