#include "editor/widgets/scene.h"
#include "stella/graphics/utils.h"
#include <vector>

namespace stella
{
namespace widget
{
  Scene::Scene() : Widget("Scene") { m_open = true; }

  void Scene::render (const ImTextureID texture_id)
  {
    ImGui::PushStyleVar (ImGuiStyleVar_WindowPadding, ImVec2 (0, 0));
    ImGui::PushStyleVar (ImGuiStyleVar_ItemSpacing, ImVec2 (2.0f, 0.0f));
    ImGui::SetNextWindowSizeConstraints (ImVec2 (896, 504), ImVec2 (897, 505));
    if (ImGui::Begin (m_name.c_str(), &m_open))
    {
      m_active = ImGui::IsWindowFocused() && ImGui::IsWindowHovered();
    const auto window_pos    = ImGui::GetWindowPos();
    const auto window_size   = ImGui::GetWindowSize();
    const float frame_height = ImGui::GetFrameHeight();
    std::vector<int> coords  = graphics::Utils::get_16x9_viewport_coords (m_widget_width, m_widget_height);

    m_widget_width  = window_size[0];
    m_widget_height = window_size[1] - frame_height;
    m_widget_x      = window_pos[0];
    m_widget_y      = window_pos[1];

    m_game_screen_width  = coords[1];
    m_game_screen_height = coords[2];

    if (m_widget_width / (float) m_widget_height > 1.78f)
    {
      m_game_screen_x_spacing = coords[0] / 2;
      m_game_screen_y_spacing = 0;
      ImGui::Dummy (ImVec2 (m_game_screen_x_spacing, 0));
      ImGui::SameLine();
    }
    else if (m_widget_width / (float) m_widget_height < 1.77f)
    {
      m_game_screen_x_spacing = 0;
      m_game_screen_y_spacing = coords[0] / 2;
      ImGui::Dummy (ImVec2 (0, m_game_screen_y_spacing));
    }

    ImGui::Image (texture_id, ImVec2 (m_game_screen_width, m_game_screen_height), ImVec2 (0, 1), ImVec2 (1, 0));
    }
    ImGui::End();
    ImGui::PopStyleVar();
    ImGui::PopStyleVar();
  }
} // namespace widget
} // namespace stella
