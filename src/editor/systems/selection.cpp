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
    const auto& pos = registry.get<component::Position>(entity);
    const auto& dim = registry.get<component::Dimension>(entity);
    auto& sel = registry.get<component::Selected>(entity);

    auto handler_x = registry.create();
    registry.emplace<component::Position>(handler_x, pos.x + dim.w/2.f, pos.y + dim.h/2.f - 4.f, pos.z);
    registry.emplace<component::Dimension>(handler_x, 128.f, 11.f);
    auto& sprite2 = registry.emplace<component::SpriteT>(handler_x, "handler-x");
    sprite2.layer = "editor";

    auto handler_y = registry.create();
    registry.emplace<component::Position>(handler_y, pos.x + dim.w/2.f - 4.f, pos.y - 128.f + dim.h/2.f, pos.z);
    registry.emplace<component::Dimension>(handler_y, 11.f, 128.f);
    auto& sprite3 = registry.emplace<component::SpriteT>(handler_y, "handler-y");
    sprite3.layer = "editor";

    auto handler_move = registry.create();
    registry.emplace<component::Position>(handler_move, pos.x + dim.w/2.f - 8.f, pos.y + dim.h/2.f - 8.f, pos.z);
    registry.emplace<component::Dimension>(handler_move, 16.f, 16.f);
    auto& sprite1 = registry.emplace<component::SpriteT>(handler_move, "handler-move");
    sprite1.layer = "editor";

    sel.handler_move = handler_move;
    sel.handler_x = handler_x;
    sel.handler_y = handler_y;
  }

  void Selection::m_remove_selection_handler (entt::registry& registry, entt::entity entity)
  {

  }
} // namespace system
} // namespace stella