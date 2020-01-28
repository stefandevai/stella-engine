#pragma once

#include "../components.h"
#include "./system.h"
#include <SDL.h>

#define MAX_PLAYER_VELOCITY 200.f

namespace stella
{
namespace graphics
{
  class Display;

}
} // namespace stella

namespace stella
{
namespace systems
{
  class PlayerSystem : public System
  {
  public:
    enum State
    {
      IDLE,
      RUNNING,
      JUMPING,
      FALLING
    };

  private:
    stella::graphics::Display& Display;
    State current_state = IDLE;
    stella::audio::SoundPlayer& SoundPlayer;

  public:
    PlayerSystem (stella::graphics::Display& display, stella::audio::SoundPlayer& sound_player)
      : Display (display), SoundPlayer (sound_player)
    {
      this->SoundPlayer.AddSound ("jump", "assets/audio/jump.ogg");
      this->SoundPlayer.AddSound ("land", "assets/audio/land.ogg");
      this->SoundPlayer.AddStream ("run", "assets/audio/run.ogg");
    }

    ~PlayerSystem() override {}

    void update (entt::registry& registry, const double dt) override
    {
      registry
          .group<components::PlayerComponent> (
              entt::get<components::Body2DComponent, components::PositionComponent, components::DimensionComponent>)
          .each ([this, &registry] (auto entity, auto& player, auto& body, auto& pos, auto& dim) {
            auto previous_state = this->current_state;

            if (body.Body->Velocity.x > MAX_PLAYER_VELOCITY)
              body.Body->Velocity.x = MAX_PLAYER_VELOCITY;
            else if (body.Body->Velocity.x < -MAX_PLAYER_VELOCITY)
              body.Body->Velocity.x = -MAX_PLAYER_VELOCITY;
            // Handle collision events
            // TODO: Use library events
            // Collided bottom
            if (body.Body->Collisions.test (2))
            {
              if (player.InAir)
              {
                this->SoundPlayer.Play ("land");
              }
              player.InAir        = false;
              this->current_state = IDLE;
            }

            // Handle input
            body.Body->Acceleration.x = 0.f;
            if (this->Display.IsKeyDown (SDL_SCANCODE_LEFT))
            {
              body.Body->Acceleration.x -= player.Acceleration;
              this->current_state = RUNNING;

              if (!player.SpriteDirection.test (0))
              {
                player.SpriteDirection.flip();
                auto sprite = registry.get<components::SpriteComponent> (entity);
                sprite.Sprite->SetScale (glm::vec2 (-1.0, 1.0));
              }
            }
            if (this->Display.IsKeyDown (SDL_SCANCODE_RIGHT))
            {
              body.Body->Acceleration.x += player.Acceleration;
              this->current_state = RUNNING;

              if (!player.SpriteDirection.test (1))
              {
                player.SpriteDirection.flip();
                auto sprite = registry.get<components::SpriteComponent> (entity);
                sprite.Sprite->SetScale (glm::vec2 (1.0, 1.0));
              }
            }

            if (this->Display.IsKeyDown (SDL_SCANCODE_UP))
            {
              if (!player.InAir)
              {
                body.Body->Velocity.y     = -player.JumpForce;
                body.Body->Acceleration.y = 0.0f;

                player.InAir = true;

                this->SoundPlayer.Play ("jump");
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

            auto& anims = registry.get<components::AnimationsComponent> (entity);
            this->SetState (this->current_state, anims, previous_state);
          });
    }

  private:
    void
    SetState (PlayerSystem::State state, components::AnimationsComponent& anims, PlayerSystem::State previous_state)
    {
      if (previous_state != this->current_state && previous_state == RUNNING)
      {
        this->SoundPlayer.Stop ("run");
      }

      switch (state)
      {
        case RUNNING:
          anims.current_animation = "run";
          this->SoundPlayer.Play ("run");
          break;
        case JUMPING:
          anims.current_animation = "jump";
          break;
        case FALLING:
          anims.current_animation = "fall";
          break;
        case IDLE:
          anims.current_animation = "idle";
          break;
        default:
          break;
      }
    }
  };
} // namespace systems
} // namespace stella
