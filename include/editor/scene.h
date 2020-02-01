#pragma once

#include "../../lib/imgui/imgui.h"

namespace stella
{
namespace editor
{
  class Scene
  {
  private:
    int m_widget_width = 0, m_widget_height = 0;

  public:
    Scene ();
    void render(const ImTextureID texture_id);
    inline int get_width() const { return m_widget_width; }
    inline int get_height() const { return m_widget_height; } 
  };
} // namespace editor
} // namespace stella
