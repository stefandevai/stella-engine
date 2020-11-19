#pragma once

#include "stella/components/npc.hpp"
#include "imgui.h"

namespace editor
{
namespace widget
{
  struct NPC
  {
    void operator() (entt::registry& registry, const entt::entity entity) {}
  };
} // namespace widget
} // namespace editor
