#pragma once

#include "widget.hpp"
// #include "stella/component_list.hpp"
#include <entt/entity/entity.hpp>
// #include "editor/widgets/components/components_widgets.hpp" // IWYU pragma: export

namespace stella
{
namespace widget
{
  struct EntityComponents
  {
    EntityComponents (entt::entity entity) { m_selected_entity = entity; }
    EntityComponents() {}

    template<class T>
    void render_component_node (entt::registry& registry,
                                const std::function<void (entt::registry&, const entt::entity)>& render_params)
    {
      if (registry.has<T> (m_selected_entity))
      {
        T& component = registry.get<T> (m_selected_entity);
        if (ImGui::TreeNode (component.name.c_str()))
        {
          render_params (registry, m_selected_entity);
          ImGui::Dummy (ImVec2 (0, 2.0));
          ImGui::TreePop();
        }
      }
    }

    void set_entity (const entt::entity entity) { m_selected_entity = entity; }

  private:
    entt::entity m_selected_entity = entt::null;
  };

} // namespace widget
} // namespace stella
