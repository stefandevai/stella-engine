#pragma once

#include "entt/entity/fwd.hpp"
#include <entt/entity/entity.hpp>
#include "../../lib/imgui/imgui.h" // IWYU pragma: export
#include "imgui_internal.h"

namespace stella
{
namespace system
{
  class Selection
  {
  public:
    entt::entity selected_entity = entt::null;

  public:
    Selection (entt::registry& registry);
    void update (entt::registry& registry, const ImGuiIO& io, const ImVec2& map_pos);

  private:
    void m_init_selection_handler (entt::registry& registry, entt::entity entity);
    void m_remove_selection_handler (entt::registry& registry, entt::entity entity);
  };
} // namespace system
} // namespace stella
