#pragma once

#include "../components/log.hpp"
#include <entt/entity/registry.hpp>

namespace stella
{
namespace system
{
  class System
  {
  public:
    virtual ~System()                                               = default;
    virtual void update (entt::registry& registry, const double dt) = 0;

  protected:
    System() = default;

    auto& get_log_component (entt::registry& registry, entt::registry::entity_type entity)
    {
      if (!registry.has<component::Log> (entity))
      {
        registry.emplace<component::Log> (entity);
      }
      return registry.get<component::Log> (entity);
    }
  };

} // namespace system
} // namespace stella
