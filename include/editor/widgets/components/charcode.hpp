#pragma once

#include "stella/components/charcode.hpp"
#include "imgui.h"

namespace editor
{
namespace widget
{
  struct Charcode
  {
    void operator() (entt::registry& registry, const entt::entity entity) {}
  };
} // namespace widget
} // namespace editor
