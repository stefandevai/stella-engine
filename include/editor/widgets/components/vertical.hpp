#pragma once

#include "stella/components/vertical.hpp"
#include "imgui.h"

namespace editor
{
namespace widget
{
  struct Vertical
  {
    void operator() (entt::registry& registry, const entt::entity entity) {}
  };
} // namespace widget
} // namespace editor
