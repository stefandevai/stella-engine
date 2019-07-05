#pragma once

#include "./system.h"
#include "../components.h"

namespace stella
{
namespace systems
{
class MovementSystem : public System
{
  public:
    MovementSystem() { }

    ~MovementSystem() override { }

    void update(entt::registry &registry, const double dt) override
    {
      registry.group<components::MovementComponent>(entt::get<components::PositionComponent>, entt::exclude<components::Body2DComponent, components::PlayerComponent>).each([&dt](auto entity, auto &mov, auto &pos)
      {
        pos.x += mov.TargetVelocity.x*dt;
        pos.y += mov.TargetVelocity.y*dt;
      });
    }
};
} // namespace systems
} // namespace stella

