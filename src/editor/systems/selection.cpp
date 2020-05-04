#include "editor/systems/selection.hpp"
#include "entt/entity/group.hpp"
#include "entt/entity/storage.hpp"
#include "entt/entity/utility.hpp"
#include "editor/components/selected.hpp"
#include "stella/components/position.hpp"
#include "stella/components/dimension.hpp"
#include "stella/components/sprite.hpp"

// TEMP
#include <iostream>
// TEMP

namespace stella
{
namespace system
{
  Selection::Selection (entt::registry& registry)
  {
    registry.on_construct<component::Selected>().connect<&Selection::m_init_selection_handler> (this);
    registry.on_destroy<component::Selected>().connect<&Selection::m_remove_selection_handler> (this);
  }

  void Selection::update (entt::registry& registry, const double dt)
  {
    registry.group<component::Selected> (entt::get<component::Position>)
        .each ([this, &registry] (auto entity, auto& selected, auto& pos)
    {


    });

    registry.group<component::Selected> (entt::get<component::Position>)
        .each ([this, &registry] (auto entity, auto& selected, auto& pos)
    {


    });
  }

  void Selection::m_init_selection_handler (entt::registry& registry, entt::entity entity)
  {
    std::cout << "HEREERERE\n";
    const auto& pos = registry.get<component::Position>(entity);
    auto& sel = registry.get<component::Selected>(entity);

    auto handler_sprite = registry.create();
    registry.emplace<component::Position>(handler_sprite, pos.x, pos.y, pos.z);
    registry.emplace<component::Dimension>(handler_sprite, 32.f, 32.f);
    auto& sprite = registry.emplace<component::SpriteT>(handler_sprite, "nikte");
    sprite.layer = "editor";
    sel.sprite = handler_sprite;
    std::cout << "HEREERERE\n";
  }

  void Selection::m_remove_selection_handler (entt::registry& registry, entt::entity entity)
  {

  }
} // namespace system
} // namespace stella