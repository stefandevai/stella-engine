#pragma once

#include "stella/components/typewriter.hpp"
#include "imgui.h"

namespace editor
{
namespace widget
{
  struct Typewriter
  {
    void operator() (entt::registry& registry, const entt::entity entity) {}
  };
} // namespace widget
} // namespace editor
