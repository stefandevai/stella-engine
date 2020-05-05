#pragma once

#include "stella/components/shape.hpp"
#include "imgui.h"

namespace stella
{
namespace widget
{
  struct Shape
  {
    void operator() (entt::registry& registry, const entt::entity entity) {}
  };
} // namespace widget
} // namespace stella