#pragma once

#include "stella/components/character_animation.hpp"
#include "imgui.h"

namespace editor
{
namespace widget
{
  struct CharacterAnimation
  {
    void operator() (entt::registry& registry, const entt::entity entity) {}
  };
} // namespace widget
} // namespace editor
