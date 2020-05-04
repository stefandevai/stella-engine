#pragma once

#include <functional>
//#include "../../lib/imgui/imgui.hpp"
#include "widget.hpp"
#include "editor/state.hpp"
#include <entt/entity/registry.hpp>

#define ICON_FA_MIN 0xf044
#define ICON_FA_MAX 0xf245
#define ICON_FA_EDIT          u8"\uf044"
#define ICON_FA_PLAY          u8"\uf04b"
#define ICON_FA_PLUS          u8"\uf067"
#define ICON_FA_MOUSE_POINTER u8"\uf245"

namespace stella
{
namespace widget
{
  class Toolbar : public Widget
  {
  private:
    ImVec2 m_size{0,0};
    const ImVec4 m_button_color{0.4f, 0.4f, 0.4f, 1.0f};
    const ImVec4 m_button_hover_color{0.1f, 0.1f, 0.1f, 1.0f};
    const ImVec4 m_button_selected_color{1.0f, 1.0f, 1.0f, 1.0f};

  private:
    void m_render_inspector_button (editor::State& state, editor::Tool& tool);
    void m_render_tilepen_button (editor::State& state, editor::Tool& tool);
    void m_render_play_button (editor::State& state, editor::Tool& tool);
    void m_render_plus_button (entt::registry& registry, const float width);
    void m_add_new_entity (entt::registry& registry, const float width);
    
  public:
    Toolbar();
    ~Toolbar();
    void render (entt::registry& registry, editor::State& state, editor::Tool& tool, const float width, std::function<void()> draw_menu_bar);
    const ImVec2& size() { return m_size; }
  };
} // namespace editor
} // namespace stella
