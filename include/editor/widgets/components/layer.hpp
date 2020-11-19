#pragma once

#include "stella/components/layer.hpp"
#include "imgui.h"

namespace editor
{
namespace widget
{
  struct Layer
  {
    void operator() (entt::registry& registry, const entt::entity entity) {}
  };
} // namespace widget
} // namespace editor
