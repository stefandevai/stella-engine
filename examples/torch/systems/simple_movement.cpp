#include "simple_movement.h"

#include "../components/game_components.h"

SimpleMovementSystem::SimpleMovementSystem() {
}

SimpleMovementSystem::~SimpleMovementSystem() {

}

void SimpleMovementSystem::update(ex::EntityManager &es, ex::EventManager &events, ex::TimeDelta dt) {
  es.each<MovementComponent, PositionComponent>([&dt](ex::Entity entity,
                                                      MovementComponent &mov,
                                                      PositionComponent &pos) {
      if (!entity.has_component<Body2DComponent>()) {
        pos.x += mov.TargetVelocity.x*dt;
        pos.y += mov.TargetVelocity.y*dt;
      }
  });
}




