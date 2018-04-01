#include "player_movement_system.h"

#include <SDL2/SDL.h>

#include "../components/body_component.h"
#include "../components/input_component.h"
#include "../components/movement_component.h"
#include "../components/spatial_component.h"

PlayerMovementSystem::PlayerMovementSystem(const int &boundx, stella::graphics::Display &display)
    : BoundX(boundx), Display(display) {}

PlayerMovementSystem::~PlayerMovementSystem() {}

void PlayerMovementSystem::update(entityx::EntityManager &es,
                                  entityx::EventManager &events,
                                  entityx::TimeDelta dt) {
  es.each<MovementComponent, BodyComponent, SpatialComponent, InputComponent>(
      [this](entityx::Entity entity, MovementComponent &mov,
             BodyComponent &body, SpatialComponent &spa,
             InputComponent &input) {
      });
}
