#include "player_movement_system.h"

#include <SDL2/SDL.h>

#include "../components/game_components.h"

PlayerMovementSystem::PlayerMovementSystem(const int &boundx, stella::graphics::Display &display)
    : BoundX(boundx), Display(display) {}

PlayerMovementSystem::~PlayerMovementSystem() {}

void PlayerMovementSystem::update(ex::EntityManager &es,
                                  ex::EventManager &events,
                                  ex::TimeDelta dt) {
  es.each<PlayerComponent, Body2DComponent, PositionComponent, DimensionComponent>(
      [this, &dt](ex::Entity entity, PlayerComponent &player,
                                          Body2DComponent &body,
                                          PositionComponent &pos,
                                          DimensionComponent &dim) {
        const float GROUND_Y = 512;

        body.Body->Acceleration.x = 0.f;
        if (this->Display.IsKeyDown(SDL_SCANCODE_LEFT)) {
          if (player.InAir)
            body.Body->Acceleration.x -= 4*player.Acceleration;
          else
            body.Body->Acceleration.x -= player.Acceleration;
        }
        if (this->Display.IsKeyDown(SDL_SCANCODE_RIGHT)) {
          if (player.InAir)
            body.Body->Acceleration.x += 4*player.Acceleration;
          else
            body.Body->Acceleration.x += player.Acceleration;
        }

        if (pos.x < 0.f) {
          pos.x = 0.f;
          body.Body->Velocity.x = 0.0f;
          body.Body->Acceleration.x = 0.0f;
        }
        else if (pos.x > 1024 - dim.w) {
          pos.x = 1024 - dim.w;
          body.Body->Velocity.x = 0.0f;
          body.Body->Acceleration.x = 0.0f;
        } 

        if (pos.y > GROUND_Y - dim.h) {
          pos.y = GROUND_Y - dim.h;
          body.Body->Velocity.y = 0.0f;
          body.Body->Acceleration.y = 0.0f;
          player.InAir = false;
          body.Body->Drag.x = player.Drag;
        }

        if (this->Display.IsKeyDown(SDL_SCANCODE_UP) && !player.InAir) {
          body.Body->Velocity.y = -player.JumpForce;
          player.InAir = true;

          // Limit player movement in air
          body.Body->Drag.x = player.Drag*2;
        }
      });
}
