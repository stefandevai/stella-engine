#pragma once

#include "./imgui/imgui.h"
#include <entt/entity/registry.hpp>

namespace stella
{
namespace editor
{
  class Inspector
  {
    private:
      entt::entity m_selected_entity = entt::null;

    public:
      Inspector();
      ~Inspector();
      void render(entt::registry& registry);
      inline void set_selected_entity (entt::entity entity) { m_selected_entity = entity; }
      inline entt::entity get_selected_entity () const {  return m_selected_entity; }
  };
}
}

