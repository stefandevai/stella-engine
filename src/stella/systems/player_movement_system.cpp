#include <SDL2/SDL.h>

#include "stella/components.h"
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
          this->current_state = IDLE;
        }

        // Handle input
        body.Body->Acceleration.x = 0.f;
        if (this->Display.IsKeyDown(SDL_SCANCODE_LEFT)) {
          body.Body->Acceleration.x -= player.Acceleration;
          this->current_state = RUNNING;

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
          this->current_state = RUNNING;

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

        if (body.Body->Velocity.y < 0.0)
        {
          this->current_state = JUMPING;
        }
        else if (body.Body->Velocity.y > 0.0)
        {
          this->current_state = FALLING;
        }

        auto anims = entity.component<components::AnimationsComponent>();
        this->SetState(this->current_state, anims);
      });
}

void PlayerMovementSystem::SetState(PlayerMovementSystem::State state, entityx::ComponentHandle<stella::components::AnimationsComponent, entityx::EntityManager> anims)
{
  switch(state)
  {
    case RUNNING:
      anims->current_animation = "run";
      break;
    case JUMPING:
      anims->current_animation = "jump";
      break;
    case FALLING:
      anims->current_animation = "fall";
      break;
    case IDLE:
      anims->current_animation = "idle";
      break;
    default:
      break;
  }
}


} // namespace systems
} // namespace stella
