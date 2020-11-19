#pragma once

#include <entt/entity/entity.hpp>

namespace editor
{
namespace component
{
  struct Selected
  {
    bool focus                = true;
    entt::entity handler_move = entt::null;
    entt::entity handler_x    = entt::null;
    entt::entity handler_y    = entt::null;
  };
} // namespace component
} // namespace editor
