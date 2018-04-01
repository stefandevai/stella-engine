#include "game_systems.h"

#include "../components/game_components.h"

MovementSystem::MovementSystem() {}

MovementSystem::~MovementSystem() {}

void MovementSystem::update(entityx::EntityManager &es,
                            entityx::EventManager &events,
                            entityx::TimeDelta dt) {
  es.each<SpatialComponent, MovementComponent>([dt](entityx::Entity entity,
                                                    SpatialComponent &spa,
                                                    MovementComponent &mov) {

  });
}
