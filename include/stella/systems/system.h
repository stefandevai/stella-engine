#pragma once

#include "../components/log.h"
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
      if (!registry.has<components::Log> (entity))
      {
        registry.assign<components::Log> (entity);
      }
      return registry.get<components::Log> (entity);
    }
  };

} // namespace systems
} // namespace stella
