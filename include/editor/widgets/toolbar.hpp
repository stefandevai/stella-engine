#pragma once

#include <functional>
//#include "../../lib/imgui/imgui.hpp"
#include "widget.hpp"
#include "editor/state.hpp"

#define ICON_FA_EDIT          u8"\uf044"
#define ICON_FA_PLAY          u8"\uf04b"
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
    
    // If the player clicked on a tool or just used a shortcut

  public:
    Toolbar();
    ~Toolbar();
    void render (editor::State& state, editor::Tool& tool, const float width, std::function<void()> draw_menu_bar);
    const ImVec2& size() { return m_size; }
  };
} // namespace editor
} // namespace stella
