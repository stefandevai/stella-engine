#include "player_movement_system.h"

#include <SDL2/SDL.h>

#include "../components/game_components.h"

PlayerMovementSystem::PlayerMovementSystem(const int &boundx, stella::graphics::Display &display)
    : BoundX(boundx), Display(display) {}

PlayerMovementSystem::~PlayerMovementSystem() {}

void PlayerMovementSystem::update(entityx::EntityManager &es,
                                  entityx::EventManager &events,
                                  entityx::TimeDelta dt) {
  es.each<PlayerComponent, MovementComponent, PositionComponent, DimensionComponent>(
      [this, &dt](entityx::Entity entity, PlayerComponent &player,
                                          MovementComponent &mov,
                                          PositionComponent &pos,
                                          DimensionComponent &dim) {

        mov.Acceleration.x = 0.f;
        if (this->Display.IsKeyDown(SDL_SCANCODE_LEFT)) {
          mov.Acceleration.x -= 400.f;
        }
        if (this->Display.IsKeyDown(SDL_SCANCODE_RIGHT)) {
          mov.Acceleration.x += 400.f;
        }

        if (pos.x < 0.f) {
          pos.x = 0.f;
          mov.Velocity.x = 0.0f;
          mov.Acceleration.x = 0.0f;
        }
        else if (pos.x > this->Display.GetWidth() - dim.w) {
          pos.x = this->Display.GetWidth() - dim.w;
          mov.Velocity.x = 0.0f;
          mov.Acceleration.x = 0.0f;
        } 
      });
}
