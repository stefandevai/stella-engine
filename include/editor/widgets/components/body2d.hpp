#pragma once

#include "stella/components/body2d.hpp"
#include "imgui.h"

namespace editor
{
namespace widget
{
  struct Body2D
  {
    void operator() (entt::registry& registry, const entt::entity entity)
    {
      auto& body           = registry.get<stella::component::Body2D> (entity);
      float movement_speed = body.movement_speed;
      bool is_static       = body.Body->IsStatic;
      ImGui::PushID ("body#inspector");
      ImGui::DragFloat ("Movement speed", &movement_speed, 1.0f, 0.0f, 0.0f);
      ImGui::Checkbox ("Static", &is_static);
      ImGui::PopID();
      body.movement_speed = movement_speed;
      body.Body->IsStatic = is_static;
    }
  };
} // namespace widget
} // namespace editor
