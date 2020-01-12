#pragma once

#include "./system.h"
#include "../components.h"
#include <iostream>
#include "stella/topdown/world.h"

namespace stella
{
namespace systems
{
class PhysicsSystem : public System
{
  private:
    const core::TileMap &m_tile_map;
    stella::topdown::World m_world{m_tile_map};

  public:
    explicit PhysicsSystem(const core::TileMap &tile_map, entt::registry &registry) : m_tile_map(tile_map)
    { 
      registry.on_destroy<components::Body2DComponent>().connect<&PhysicsSystem::remove_body_from_world>(this);
    }

    ~PhysicsSystem() override { }

    void update(entt::registry &registry, const double dt) override
    {
      registry.group<components::Body2DComponent>(entt::get<components::PositionComponent, components::DimensionComponent>).each([this, &registry](auto entity, auto &body, auto &pos, auto &dim)
      {
        if (!body.Initialized)
        {
          auto &log_component = get_log_component(registry, entity);
          log_component.log("Initialized body");

          body.Body = std::make_shared<stella::topdown::Body>(glm::vec2(pos.x, pos.y), glm::vec2(dim.w, dim.h), glm::vec2(body.Drag[0], body.Drag[1]), body.CollideWithBorders);
          if (registry.has<components::MovementComponent>(entity))
          {
            auto mov = registry.get<components::MovementComponent>(entity);
            body.Body->TargetVelocity = mov.TargetVelocity;
            //body.Body->Gravity = mov.Gravity;

            if (mov.ConstantVelocity)
            {
              body.Body->Velocity = mov.TargetVelocity;
            }
          }
          else
          {
            body.Body->IsStatic = true;
          }

          this->m_world.AddBody(body.Body);
          body.Initialized = true;
        }
        else
        {
          const glm::vec2& new_position = body.Body->GetPosition();
          pos.x = new_position.x;
          pos.y = new_position.y;
        }
      });

      this->m_world.Update(dt);
    }

  private:
    PhysicsSystem() = delete;

    void remove_body_from_world(entt::registry &registry, entt::entity entity)
    {
      auto& body = registry.get<components::Body2DComponent>(entity);
      m_world.RemoveBody(body.Body);
    }
};
} // namespace systems
} // namespace stella

