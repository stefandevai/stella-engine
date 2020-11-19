#pragma once

#include "stella/components/script.hpp"
#include "imgui.h"

namespace editor
{
namespace widget
{
  struct Script
  {
    void operator() (entt::registry& registry, const entt::entity entity) {}
  };
} // namespace widget
} // namespace editor
