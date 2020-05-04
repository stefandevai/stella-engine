#pragma once

#include "entt/entity/fwd.hpp"
#include "../../lib/imgui/imgui.h" // IWYU pragma: export
#include "imgui_internal.h"

namespace stella
{
namespace system
{
  class Selection
  {
  public:
    Selection (entt::registry& registry);
    void update (entt::registry& registry, const ImGuiIO& io);

  private:
    void m_init_selection_handler (entt::registry& registry, entt::entity entity);
    void m_remove_selection_handler (entt::registry& registry, entt::entity entity);
  };
} // namespace system
} // namespace stella
