#pragma once

#include <functional>
//#include "../../lib/imgui/imgui.hpp"
#include "widget.hpp"
#include "editor/modes.hpp"
#include "editor/tools.hpp"
#include "editor/icons.hpp"
#include <entt/entity/registry.hpp>
#include "new_entity.hpp"

namespace editor
{
namespace widget
{
  class Toolbar : public Widget
  {
  private:
    ImVec2 m_size{0, 0};
    const ImVec4 m_button_color{0.4f, 0.4f, 0.4f, 1.0f};
    const ImVec4 m_button_hover_color{0.1f, 0.1f, 0.1f, 1.0f};
    const ImVec4 m_button_selected_color{1.0f, 1.0f, 1.0f, 1.0f};
    NewEntity m_new_entity{};

  private:
    void m_render_inspector_button (editor::EditorMode& mode, editor::EditorTool& tool);
    void m_render_tilepen_button (editor::EditorMode& mode, editor::EditorTool& tool);
    void m_render_play_button (editor::EditorMode& mode, editor::EditorTool& tool);
    void m_render_plus_button (entt::registry& registry, const float width);
    void m_add_new_entity (entt::registry& registry, const float width);

  public:
    Toolbar();
    ~Toolbar();
    void render (entt::registry& registry, editor::EditorMode& mode, editor::EditorTool& tool, const float width);
    const ImVec2& size() { return m_size; }
  };
} // namespace widget
} // namespace editor
