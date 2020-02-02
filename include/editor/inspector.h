#pragma once

#include "../../lib/imgui/imgui.h"
#include <entt/entity/registry.hpp>

namespace stella
{
namespace editor
{
  class Inspector
  {
  private:
    entt::entity m_selected_entity = entt::null;
    bool m_open = true;

  public:
    Inspector();
    ~Inspector();
    void render (entt::registry& registry);
    inline void set_selected_entity (entt::entity entity) { m_selected_entity = entity; }
    inline void toggle() { m_open = !m_open; }
    inline entt::entity get_selected_entity() const { return m_selected_entity; }
    inline bool is_open() const { return m_open; }
  };
} // namespace editor
} // namespace stella
