#pragma once

#include "widget.h"
#include "../../lib/imgui/imgui.h"

namespace stella
{
namespace widget
{
  class Scene : public Widget
  {
  private:
    int m_widget_width = 0, m_widget_height = 0, m_widget_x = 0, m_widget_y = 0, m_game_screen_width = 0,
        m_game_screen_height = 0, m_game_screen_x_spacing = 0, m_game_screen_y_spacing = 0;
    // Wether or not the scene window is hovered and focused
    bool m_active = false;

  public:
    Scene();
    void render (const ImTextureID texture_id);
    inline int get_width() const { return m_widget_width; }
    inline int get_height() const { return m_widget_height; }
    inline int get_x() const { return m_widget_x; }
    inline int get_y() const { return m_widget_y; }
    inline int get_game_screen_width() const { return m_game_screen_width; }
    inline int get_game_screen_height() const { return m_game_screen_height; }
    inline int get_game_screen_x_spacing() const { return m_game_screen_x_spacing; }
    inline int get_game_screen_y_spacing() const { return m_game_screen_y_spacing; }
    inline const bool active() const { return m_active; }
  };
} // namespace widget
} // namespace stella
