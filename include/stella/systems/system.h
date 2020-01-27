#pragma once

#include "../components/log_component.h"
#include <entt/entity/registry.hpp>

namespace stella
{
namespace systems
{
  class System
  {
  public:
    virtual ~System() = default;
    virtual void update (entt::registry& registry, const double dt) {}

  protected:
    System() = default;

    auto& get_log_component (entt::registry& registry, entt::registry::entity_type entity)
    {
      if (!registry.has<components::LogComponent> (entity))
      {
        registry.assign<components::LogComponent> (entity);
      }
      return registry.get<components::LogComponent> (entity);
    }
  };

} // namespace systems
} // namespace stella
