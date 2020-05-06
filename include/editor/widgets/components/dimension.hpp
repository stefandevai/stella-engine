#pragma once

#include "stella/components/dimension.hpp"
#include "imgui.h"

namespace stella
{
namespace widget
{
  struct Dimension
  {
    void operator() (entt::registry& registry, const entt::entity entity)
    {
      auto& dim          = registry.get<component::Dimension> (entity);
      float dim_input[2] = {0.0f, 0.0f};
      dim_input[0]       = dim.w;
      dim_input[1]       = dim.h;
      ImGui::PushID ("dimension#inspector");
      ImGui::DragFloat ("width", &dim_input[0], 1.0f, 0.0f, 0.0f, "%.3f");
      ImGui::DragFloat ("height", &dim_input[1], 1.0f, 0.0f, 0.0f, "%.3f");
      ImGui::PopID();
      dim.w = dim_input[0];
      dim.h = dim_input[1];
    }
  };
} // namespace widget
} // namespace stella