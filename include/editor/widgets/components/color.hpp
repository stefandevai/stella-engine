#pragma once

#include "stella/components/color.hpp"
#include "imgui.h"

namespace editor
{
namespace widget
{
  struct Color
  {
    void operator() (entt::registry& registry, const entt::entity entity) {}
  };
} // namespace widget
} // namespace editor
