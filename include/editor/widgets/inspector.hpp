#pragma once

#include "widget.hpp"
//#include "../../lib/imgui/imgui.hpp"
#include <entt/entity/registry.hpp> // IWYU pragma: export
#include "add_components.hpp"

namespace stella
{
namespace widget
{
  class Inspector : public Widget
  {
  private:
    entt::entity m_selected_entity = entt::null;
    AddComponents m_add_components{};

    void m_render_component_nodes (entt::registry& registry);

    template <class T>
    void m_render_component_node (entt::registry& registry, std::function<void(T&)> render_params)
    {
      if (registry.has<T> (m_selected_entity))
      {
        T& component = registry.get<T>(m_selected_entity);
        if (ImGui::TreeNode(component.name.c_str()))
        {
          render_params(component);
          ImGui::Dummy(ImVec2(0, 2.0));
          ImGui::Separator();
          ImGui::Dummy(ImVec2(0, 2.0));
          ImGui::TreePop();
        }
      }
    }

  public:
    Inspector();
    void render (entt::registry& registry);
    inline void set_selected_entity (entt::entity entity) { m_selected_entity = entity; }
    inline entt::entity get_selected_entity() const { return m_selected_entity; }
  };
} // namespace widget
} // namespace stella
