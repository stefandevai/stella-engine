#pragma once

#include "widget.h"
#include "../../lib/imgui/imgui.h"
#include <entt/entity/registry.hpp>

namespace stella
{
namespace widget
{
  class Inspector : public Widget
  {
  private:
    entt::entity m_selected_entity = entt::null;

  public:
    Inspector();
    void render (entt::registry& registry);
    inline void set_selected_entity (entt::entity entity) { m_selected_entity = entity; }
    inline entt::entity get_selected_entity() const { return m_selected_entity; }
  };
} // namespace widget
} // namespace stella
