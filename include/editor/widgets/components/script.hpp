#pragma once

#include "stella/components/script.hpp"
#include "imgui.h"

namespace stella
{
namespace widget
{
  struct Script
  {
    void operator() (entt::registry& registry, const entt::entity entity) {}
  };
} // namespace widget
} // namespace stella