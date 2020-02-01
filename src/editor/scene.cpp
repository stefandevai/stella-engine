#include "editor/scene.h"
#include "stella/graphics/utils.h"
#include <vector>

namespace stella
{
namespace editor
{
    Scene::Scene ()
    {

    }

    void Scene::render(const ImTextureID texture_id)
    {
      ImGui::PushStyleVar (ImGuiStyleVar_WindowPadding, ImVec2(0,0));
      ImGui::Begin("Scene", nullptr, ImGuiWindowFlags_NoTitleBar);
      const auto window_size = ImGui::GetWindowSize();
      const float frame_height = ImGui::GetFrameHeight();
      m_widget_width = window_size[0];
      m_widget_height = window_size[1] - frame_height;

      std::vector<int> coords = graphics::Utils::get_16x9_viewport_coords(m_widget_width, m_widget_height);
      
      if (m_widget_width / (float) m_widget_height > 1.78f)
      {
          ImGui::Dummy(ImVec2(coords[0]/2, 0));
          ImGui::SameLine();
      }
      else if (m_widget_width / (float) m_widget_height < 1.77f)
      {
          ImGui::Dummy(ImVec2(0, coords[0]/2));
      }
      
      ImGui::Image(texture_id, ImVec2(coords[1], coords[2]), ImVec2(0,1), ImVec2(1,0));
      ImGui::End();
      ImGui::PopStyleVar ();
      
      // Will be used to update the viewport
      
    }
} // namespace editor
} // namespace stella
