#include <memory>
#include <glm/glm.hpp>

#include "stella/components/game_components.h"
#include "stella/systems/physics_system.h"

namespace stella {
namespace systems {
PhysicsSystem::PhysicsSystem() {
  this->World.SetGravity(2200.f);
}

PhysicsSystem::~PhysicsSystem() {

}

void PhysicsSystem::update(ex::EntityManager &es, ex::EventManager &events, ex::TimeDelta dt) {
  es.each<components::Body2DComponent, components::PositionComponent, components::DimensionComponent>([this, &es](ex::Entity entity,
                                                                  components::Body2DComponent &body,
                                                                  components::PositionComponent &pos,
                                                                  components::DimensionComponent &dim) {
    if (!body.Initialized) {
      body.Body = std::make_shared<stella::physics2d::Body>(glm::vec2(pos.x, pos.y), glm::vec2(dim.w, dim.h), glm::vec2(body.Drag[0], body.Drag[1]), body.CollideWithBorders);
      if (entity.has_component<components::MovementComponent>()) {
        auto mov = entity.component<components::MovementComponent>();
        body.Body->TargetVelocity = mov->TargetVelocity;
        body.Body->Gravity = mov->Gravity;

        if (mov->ConstantVelocity) {
          body.Body->Velocity = mov->TargetVelocity;
        }
      }
      else {
        body.Body->IsStatic = true;
      }

      this->World.AddBody(body.Body);
      body.Initialized = true;
    }
    else {
      const glm::vec2& new_position = body.Body->GetPosition();
      pos.x = new_position.x;
      pos.y = new_position.y;
    }
  });

  this->World.Update(dt);
}
} // namespace systems
} // namespace stella

