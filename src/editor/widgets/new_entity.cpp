#include "editor/widgets/new_entity.hpp"

namespace stella
{
namespace widget
{
    NewEntity::NewEntity ()
    : Widget ("new-entity")
    {

    }

    void NewEntity::render (entt::registry& registry, const float width)
    {
      if (m_open)
      {
        ImGui::SetNextWindowPos(ImVec2(width/2.f - width*0.7f/2.f, 30.f), ImGuiCond_Once);
        ImGui::SetNextWindowSize(ImVec2(width*0.7f, 500.f), ImGuiCond_Once);
        ImGui::Begin("Create a new Entity", &m_open);

        ImGui::Text("Select Components:");
        ImGui::Dummy (ImVec2 (0.f, 10.f));
        if (ImGui::MenuItem("Body2D")) { }
        if (ImGui::MenuItem("Color")) { }
        if (ImGui::MenuItem("Dimension")) { }
        if (ImGui::MenuItem("Position")) { }
        if (ImGui::MenuItem("Sprite")) { }
        if (ImGui::MenuItem("Tile")) { }
        
        ImGui::Dummy (ImVec2 (0.f, 10.f));
        ImGui::Separator();
        ImGui::Dummy (ImVec2 (0.f, 20.f));

        if (ImGui::Button("Create!", ImVec2(100, 40)))
        {
            auto entity = registry.create();
            // add_components (entity, m_component_menu.components);
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel", ImVec2(100, 40)))
        {
            m_open = false;
        }
        ImGui::End();
      }

    }
} // namespace editor
} // namespace stella
