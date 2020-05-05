#include "editor/systems/selection.hpp"
#include "entt/entity/group.hpp"
#include "entt/entity/storage.hpp"
#include "entt/entity/utility.hpp"
#include "editor/components/selected.hpp"
#include "stella/components/position.hpp"
#include "stella/components/dimension.hpp"
#include "stella/components/color.hpp"
#include "stella/components/sprite.hpp"
#include "stella/components/camera.hpp"
#include "stella/components/in_group.hpp"
#include <entt/entity/registry.hpp>

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

  void Selection::update (entt::registry& registry, const ImGuiIO& io, const ImVec2& map_pos)
  {
    registry.group<component::Selected> (entt::get<component::Position, component::Dimension>)
        .each ([this, &registry, &io] (auto entity, auto& sel, const auto& pos, const auto& dim)
    {
      auto& pos_handler_x = registry.get<component::Position>(sel.handler_x);
      auto& pos_handler_y = registry.get<component::Position>(sel.handler_y);
      auto& pos_handler_move = registry.get<component::Position>(sel.handler_move);

      pos_handler_x.x = pos.x + dim.w/2.f;
      pos_handler_x.y = pos.y + dim.h/2.f - 4.f;

      pos_handler_y.x = pos.x + dim.w/2.f - 4.f;
      pos_handler_y.y = pos.y - 52.f + dim.h/2.f;

      pos_handler_move.x = pos.x + dim.w/2.f - 4.f;
      pos_handler_move.y = pos.y + dim.h/2.f - 4.f;
    });

    if (ImGui::IsMouseClicked (0))
    {
      entt::entity clicked_entity = entt::null;
      registry.view<component::Position, component::Dimension> (entt::exclude<component::Camera, component::InGroup>)
          .each ([this, &registry, &map_pos, &clicked_entity] (auto entity, auto& pos, const auto& dim)
      {
        if (registry.valid(entity) && selected_entity != entity)
        {
          const auto &mpos = map_pos;
          // If mouse pos is on entity
          if (mpos.x >= pos.x && mpos.x < pos.x + dim.w && mpos.y >= pos.y && mpos.y < pos.y + dim.h)
          {
            registry.emplace<component::Selected>(entity);

            if (registry.valid(selected_entity) && selected_entity != entt::null)
            {
              registry.remove_if_exists<component::Selected>(selected_entity);
            }
            selected_entity = entity;
            return;
          }
        }
      });
    }
  }

  void Selection::m_init_selection_handler (entt::registry& registry, entt::entity entity)
  {
    // if (selected_entity != entity)
    // {
    //   if (registry.valid(selected_entity) && selected_entity != entt::null)
    //   {
    //     registry.remove_if_exists<component::Selected>(selected_entity);
    //   }
    //   selected_entity = entity;
    // }
    const auto& pos = registry.get<component::Position>(entity);
    const auto& dim = registry.get<component::Dimension>(entity);
    auto& sel = registry.get<component::Selected>(entity);

    auto handler_move = registry.create();
    registry.emplace<component::Position>(handler_move, pos.x + dim.w/2.f - 4.f, pos.y + dim.h/2.f - 4.f, pos.z + 1);
    registry.emplace<component::Dimension>(handler_move, 8.f, 8.f);
    registry.emplace<component::Color>(handler_move, "#ffffff88");
    auto& sprite1 = registry.emplace<component::SpriteT>(handler_move, "handler-move");
    sprite1.layer = "editor";

    auto handler_x = registry.create();
    registry.emplace<component::Position>(handler_x, pos.x + dim.w/2.f, pos.y + dim.h/2.f - 4.f, pos.z);
    registry.emplace<component::Dimension>(handler_x, 52.f, 11.f);
    registry.emplace<component::Color>(handler_x, "#ffffff88");
    auto& sprite2 = registry.emplace<component::SpriteT>(handler_x, "handler-x");
    sprite2.layer = "editor";

    auto handler_y = registry.create();
    registry.emplace<component::Position>(handler_y, pos.x + dim.w/2.f - 4.f, pos.y - 52.f + dim.h/2.f, pos.z);
    registry.emplace<component::Dimension>(handler_y, 11.f, 52.f);
    registry.emplace<component::Color>(handler_y, "#ffffff88");
    auto& sprite3 = registry.emplace<component::SpriteT>(handler_y, "handler-y");
    sprite3.layer = "editor";

    sel.handler_move = handler_move;
    sel.handler_x = handler_x;
    sel.handler_y = handler_y;
  }

  void Selection::m_remove_selection_handler (entt::registry& registry, entt::entity entity)
  {
    auto& sel = registry.get<component::Selected>(entity);
    if (registry.valid(sel.handler_move))
    {
      registry.destroy(sel.handler_move);
    }
    if (registry.valid(sel.handler_x))
    {
      registry.destroy(sel.handler_x);
    }
    if (registry.valid(sel.handler_y))
    {
      registry.destroy(sel.handler_y);
    }
  }
} // namespace system
} // namespace stella