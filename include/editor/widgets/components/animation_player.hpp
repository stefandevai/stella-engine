#pragma once

#include "stella/components/animation_player.hpp"
#include "imgui.h"

namespace editor
{
namespace widget
{
  struct AnimationPlayer
  {
    void operator() (entt::registry& registry, const entt::entity entity) {}
  };
} // namespace widget
} // namespace editor
