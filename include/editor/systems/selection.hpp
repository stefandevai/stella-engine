#pragma once

#include "entt/entity/fwd.hpp"
#include "stella/systems/system.hpp"

namespace stella
{
namespace system
{
  class Selection : public System
  {
  public:
    Selection (entt::registry& registry);
    void update (entt::registry& registry, const double dt) override;

  private:
    void m_init_selection_handler (entt::registry& registry, entt::entity entity);
    void m_remove_selection_handler (entt::registry& registry, entt::entity entity);
  };
} // namespace system
} // namespace stella
