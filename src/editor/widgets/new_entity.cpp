#include "editor/widgets/new_entity.hpp"
#include "editor/components/selected.hpp"

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

        m_component_menu.render();
        
        ImGui::Dummy (ImVec2 (0.f, 10.f));
        ImGui::Separator();
        ImGui::Dummy (ImVec2 (0.f, 20.f));

        if (ImGui::Button("Create!", ImVec2(100, 40)))
        {
            const auto& selected_components = m_component_menu.get_map();
            auto entity = registry.create();
            for (const auto& component : selected_components)
            {
              if (component.second)
              {
                ComponentList::emplace_default_component (component.first, entity, registry);
              }
            }
            registry.emplace<component::Selected>(entity);
            // add_components (entity, m_component_menu.components);
            m_component_menu.reset();
            m_open = false;
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel", ImVec2(100, 40)))
        {
            m_open = false;
            m_component_menu.reset();
        }
        ImGui::End();
      }

    }
} // namespace editor
} // namespace stella
