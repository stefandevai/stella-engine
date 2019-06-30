#include <SDL2/SDL.h>

#include "stella/components/game_components.h"
#include "stella/systems/player_movement_system.h"

#define MAX_PLAYER_VELOCITY 200.f

namespace stella {
namespace systems {
PlayerMovementSystem::PlayerMovementSystem(const int &boundx, stella::graphics::Display &display)
    : BoundX(boundx), Display(display) {}

PlayerMovementSystem::~PlayerMovementSystem() {}

void PlayerMovementSystem::update(ex::EntityManager &es,
                                  ex::EventManager &events,
                                  ex::TimeDelta dt) {
  es.each<components::PlayerComponent, components::Body2DComponent, components::PositionComponent, components::DimensionComponent>(
      [this, &dt](ex::Entity entity, components::PlayerComponent &player,
                                          components::Body2DComponent &body,
                                          components::PositionComponent &pos,
                                          components::DimensionComponent &dim) {

        if (body.Body->Velocity.x > MAX_PLAYER_VELOCITY) body.Body->Velocity.x = MAX_PLAYER_VELOCITY;
        else if (body.Body->Velocity.x < -MAX_PLAYER_VELOCITY) body.Body->Velocity.x = -MAX_PLAYER_VELOCITY;
        // Handle collision events
        // TODO: Use library events
        // Collided bottom
        if (body.Body->Collisions.test(2)) {
          player.InAir = false;
        }

        // Handle input
        body.Body->Acceleration.x = 0.f;
        if (this->Display.IsKeyDown(SDL_SCANCODE_LEFT)) {
          body.Body->Acceleration.x -= player.Acceleration;

          //body.Body->Velocity.y = -player.JumpForce;
          //body.Body->Acceleration.y = 0.0f;

          if (!player.SpriteDirection.test(0)) {
            player.SpriteDirection.flip();
            auto player_sprite = entity.component<components::SpriteComponent>();
            player_sprite->Sprite->SetDirectFrame(1);
          }
        }
        if (this->Display.IsKeyDown(SDL_SCANCODE_RIGHT)) {
          body.Body->Acceleration.x += player.Acceleration;

          //body.Body->Velocity.y = -player.JumpForce;
          //body.Body->Acceleration.y = 0.0f;

          if (!player.SpriteDirection.test(1)) {
            player.SpriteDirection.flip();
            auto player_sprite = entity.component<components::SpriteComponent>();
            player_sprite->Sprite->SetDirectFrame(0);
          }
        }

        if (this->Display.IsKeyDown(SDL_SCANCODE_UP)) {
          if (!player.InAir) {
            body.Body->Velocity.y = -player.JumpForce;
            body.Body->Acceleration.y = 0.0f;

            player.InAir = true;
          }
        }
      });
}
} // namespace systems
} // namespace stella
