#pragma once

#include "widget.hpp"
#include <entt/entity/entity.hpp>
#include <entt/entity/registry.hpp>
#include "component_menu.hpp"

namespace stella
{
namespace widget
{
  struct NewEntity : public Widget
  {
    NewEntity();
    void render (entt::registry& registry, const float width);

  private:
    ComponentMenu m_component_menu{};
  };
} // namespace widget
} // namespace stella
