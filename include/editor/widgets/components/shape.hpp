#pragma once

#include "stella/components/shape.hpp"
#include "imgui.h"

namespace editor
{
namespace widget
{
  struct Shape
  {
    void operator() (entt::registry& registry, const entt::entity entity) {}
  };
} // namespace widget
} // namespace editor
