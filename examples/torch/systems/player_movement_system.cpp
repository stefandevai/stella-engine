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
        const float JUMP_FORCE = 200.f;
        const float ACCELERATION = 400.f;
        const float DRAG = 500.f;
        const float GROUND_Y = 368.f;

        mov.Acceleration.x = 0.f;
        if (this->Display.IsKeyDown(SDL_SCANCODE_LEFT)) {
          if (player.InAir)
            mov.Acceleration.x -= 4*ACCELERATION;
          else
            mov.Acceleration.x -= ACCELERATION;
        }
        if (this->Display.IsKeyDown(SDL_SCANCODE_RIGHT)) {
          if (player.InAir)
            mov.Acceleration.x += 4*ACCELERATION;
          else
            mov.Acceleration.x += ACCELERATION;
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

        if (pos.y > GROUND_Y - dim.h) {
          pos.y = GROUND_Y - dim.h;
          mov.Velocity.y = 0.0f;
          mov.Acceleration.y = 0.0f;
          player.InAir = false;
          mov.Drag.x = DRAG;
        }

        if (this->Display.IsKeyDown(SDL_SCANCODE_UP) && !player.InAir) {
          mov.Velocity.y = -JUMP_FORCE;
          player.InAir = true;

          // Limit player movement in air
          mov.Drag.x = DRAG*2;
        }
      });
}
