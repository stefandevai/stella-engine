#include "physics_system.h"

#include <glm/glm.hpp>

PhysicsSystem::PhysicsSystem() {

}

PhysicsSystem::~PhysicsSystem() {

}

void PhysicsSystem::update(ex::EntityManager &es, ex::EventManager &events, ex::TimeDelta dt) {
	es.each<Body2DComponent, PositionComponent, DimensionComponent>([this, &es](ex::Entity entity,
	                                                                Body2DComponent &body,
	                                                                PositionComponent &pos,
	                                                                DimensionComponent &dim) {
    if (!body.Initialized) {
      body.Body = std::make_shared<stella::physics2d::Body>(glm::vec2(pos.x, pos.y), glm::vec2(dim.w, dim.h));
      if (entity.has_component<MovementComponent>()) {
        auto mov = entity.component<MovementComponent>();
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

