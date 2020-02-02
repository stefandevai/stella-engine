#pragma once

#include "../components.h"
#include "./system.h"

namespace stella
{
namespace systems
{
  class MovementSystem : public System
  {
  public:
    MovementSystem() {}

    ~MovementSystem() override {}

    void update (entt::registry& registry, const double dt) override
    {
      registry
          .group<components::Movement> (
              entt::get<components::Position>,
              entt::exclude<components::Body2D, components::Player>)
          .each ([&dt] (auto entity, auto& mov, auto& pos) {
            pos.x += mov.TargetVelocity.x * dt;
            pos.y += mov.TargetVelocity.y * dt;
          });
    }
  };
} // namespace systems
} // namespace stella
