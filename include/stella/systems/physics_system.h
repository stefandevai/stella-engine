#pragma once

#include "./system.h"
#include "../components.h"
#include <iostream>

namespace stella
{
namespace systems
{
class PhysicsSystem : public System
{
  private:
    stella::physics2d::World m_world;
    entt::registry::entity_type m_camera;

  public:
    PhysicsSystem(entt::registry &registry, entt::registry::entity_type camera) : m_camera(camera)
    { 
      // TODO: Post an issue about not being able to add the listener
      //registry.on_destroy<components::Body2DComponent>().connect<&PhysicsSystem::remove_body_from_world>(this);
      this->m_world.SetGravity(2200.f);
    }

    ~PhysicsSystem() override { }

    void update(entt::registry &registry, const double dt) override
    {
      registry.group<components::Body2DComponent>(entt::get<components::PositionComponent, components::DimensionComponent>).each([this, &registry](auto entity, auto &body, auto &pos, auto &dim)
      {
        if (!body.Initialized)
        {
          body.Body = std::make_shared<stella::physics2d::Body>(glm::vec2(pos.x, pos.y), glm::vec2(dim.w, dim.h), glm::vec2(body.Drag[0], body.Drag[1]), body.CollideWithBorders);
          if (registry.has<components::MovementComponent>(entity))
          {
            auto mov = registry.get<components::MovementComponent>(entity);
            body.Body->TargetVelocity = mov.TargetVelocity;
            body.Body->Gravity = mov.Gravity;

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

      const auto &pos = registry.get<components::PositionComponent>(m_camera);
      if (pos.x > 0.f || pos.y > 0.f) this->m_world.SetCameraOffset(pos.x, pos.y);

      this->m_world.Update(dt);
    }

  private:
    PhysicsSystem() = delete;

    void remove_body_from_world(entt::registry &registry, entt::entity entity, components::Body2DComponent &body)
    {
      m_world.RemoveBody(body.Body);
    }
};
} // namespace systems
} // namespace stella

