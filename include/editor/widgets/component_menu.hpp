#pragma once
#include "widget.hpp"
#include <unordered_map>
#include "stella/component_list.hpp"
#include <entt/entity/entity.hpp>

namespace editor
{
namespace widget
{
  class ComponentMenu : public Widget
  {
    using ComponentMap = std::unordered_map<stella::Components, bool>;

  public:
    ComponentMenu();
    void render (entt::entity entity = entt::null);
    void reset();
    inline const ComponentMap get_map() const { return m_selected_components; };

  private:
    ComponentMap m_selected_components;
  };

} // namespace widget
} // namespace editor
