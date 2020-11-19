#pragma once

#include "stella/components/timer.hpp"
#include "imgui.h"

namespace editor
{
namespace widget
{
  struct Timer
  {
    void operator() (entt::registry& registry, const entt::entity entity) {}
  };
} // namespace widget
} // namespace editor
