#pragma once

#include "stella/components/camera.hpp"
#include "imgui.h"

namespace editor
{
namespace widget
{
  struct Camera
  {
    void operator() (entt::registry& registry, const entt::entity entity) {}
  };
} // namespace widget
} // namespace editor
