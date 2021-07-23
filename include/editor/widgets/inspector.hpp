#pragma once

#include "widget.hpp"
//#include "../../lib/imgui/imgui.hpp"
#include <entt/entity/registry.hpp> // IWYU pragma: export
#include "add_components.hpp"
#include "editor/widgets/entity_components.hpp"
/* #include "components/group.hpp" */

namespace editor
{
namespace widget
{
  class Inspector : public Widget
  {
  private:
    entt::entity m_selected_entity = entt::null;
    AddComponents m_add_components{};
    EntityComponents m_components;
    /* Group m_group_widget; */
    // int teste = 0;

    void m_render_component_nodes (entt::registry& registry, const std::vector<std::string>& texture_list);

  public:
    Inspector();
    void render (entt::registry& registry, const std::vector<std::string>& texture_list);
    inline void set_selected_entity (entt::entity entity) { m_selected_entity = entity; }
    inline entt::entity get_selected_entity() const { return m_selected_entity; }
  };
} // namespace widget
} // namespace editor
