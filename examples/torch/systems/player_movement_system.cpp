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

        body.Body->Acceleration.x = 0.f;
        if (this->Display.IsKeyDown(SDL_SCANCODE_LEFT)) {
          body.Body->Acceleration.x -= player.Acceleration;

          if (!player.SpriteDirection.test(0)) {
            player.SpriteDirection.flip();
            auto player_sprite = entity.component<SpriteComponent>();
            player_sprite->Sprite->SetDirectFrame(1);
          }
        }
        if (this->Display.IsKeyDown(SDL_SCANCODE_RIGHT)) {
          body.Body->Acceleration.x += player.Acceleration;

          if (!player.SpriteDirection.test(1)) {
            player.SpriteDirection.flip();
            auto player_sprite = entity.component<SpriteComponent>();
            player_sprite->Sprite->SetDirectFrame(0);
          }
        }

        if (this->Display.IsKeyDown(SDL_SCANCODE_UP)) {
          body.Body->Velocity.y = -player.JumpForce;
          body.Body->Acceleration.y = 0.0f;
        }
      });
}
