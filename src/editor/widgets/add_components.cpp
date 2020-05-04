#include "editor/widgets/add_components.hpp"
#include "editor/components/selected.hpp"

namespace stella
{
namespace widget
{
    AddComponents::AddComponents ()
    : Widget ("new-entity")
    {

    }

    void AddComponents::render (entt::registry& registry, entt::entity entity)
    {
      if (m_open)
      {
        ImGui::Begin("Add components to Entity", &m_open);
        m_component_menu.render();
        ImGui::Dummy (ImVec2 (0.f, 20.f));

        if (ImGui::Button("Select", ImVec2(100, 27)))
        {
            const auto& selected_components = m_component_menu.get_map();
            for (const auto& component : selected_components)
            {
              if (component.second)
              {
                ComponentList::emplace_default_component (component.first, entity, registry);
              }
            }
            m_component_menu.reset();
            m_open = false;
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel", ImVec2(100, 27)))
        {
            m_open = false;
            m_component_menu.reset();
        }
        ImGui::End();
      }

    }
} // namespace editor
} // namespace stella
