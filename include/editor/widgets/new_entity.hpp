#pragma once

#include "widget.hpp"
#include <entt/entity/entity.hpp>
#include <entt/entity/registry.hpp>
#include <vector>
#include "stella/components/component.hpp"

namespace stella
{
namespace widget
{
  struct NewEntity : public Widget
  {
    NewEntity ();
    void render (entt::registry& registry, const float width);

    private:
        std::vector<component::Component> components_to_add;
  };
} // namespace editor
} // namespace stella
