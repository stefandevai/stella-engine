#pragma once
#include "widget.hpp"
#include <entt/entity/entity.hpp>
#include <entt/entity/registry.hpp>
#include "component_menu.hpp"

namespace stella
{
namespace widget
{

  class AddComponents : public Widget
  {
    public:
      AddComponents ();
      void render(entt::registry& registry, entt::entity entity);

    private:
        ComponentMenu m_component_menu{};
  };

} // namespace widget
} // namespace stella

