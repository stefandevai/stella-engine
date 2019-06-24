#include "stella/components/game_components.h"
#include "stella/systems/simple_movement.h"

namespace stella {
namespace systems {
SimpleMovementSystem::SimpleMovementSystem() {
}

SimpleMovementSystem::~SimpleMovementSystem() {

}

void SimpleMovementSystem::update(ex::EntityManager &es, ex::EventManager &events, ex::TimeDelta dt) {
  es.each<components::MovementComponent, components::PositionComponent>([&dt](ex::Entity entity,
                                                      components::MovementComponent &mov,
                                                      components::PositionComponent &pos) {
      if (!entity.has_component<components::Body2DComponent>()) {
        pos.x += mov.TargetVelocity.x*dt;
        pos.y += mov.TargetVelocity.y*dt;
      }
  });
}
} // namespace systems
} // namespace stella

