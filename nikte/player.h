#pragma once

#include <stella/components.h>
#include <SDL2/SDL.h>

#define MAX_PLAYER_VELOCITY 200.f

namespace stella
{
namespace graphics
{

  class Display;

}
}

class Player
{
  public:
    entt::registry &m_registry;
    enum State {IDLE, RUNNING, JUMPING, FALLING};
    const entt::registry::entity_type entity = m_registry.create();
  private:
    stella::graphics::Display &Display;
    State current_state = IDLE;

  public:
    Player(entt::registry &registry, stella::graphics::Display &display) 
      : m_registry(registry), Display(display)
    { 
      
    }

    ~Player() { }

    void update()
    {
      auto &player = m_registry.get<stella::components::PlayerComponent>(entity);
      auto previous_state = this->current_state;

      // Handle input
      
      if (this->Display.IsKeyDown(SDL_SCANCODE_LEFT)) {
        this->current_state = RUNNING;

        if (!player.SpriteDirection.test(0)) {
          player.SpriteDirection.flip();
          auto sprite = m_registry.get<stella::components::SpriteComponent>(entity);
          sprite.Sprite->SetScale(glm::vec2(-1.0,1.0));
        }
      }
      if (this->Display.IsKeyDown(SDL_SCANCODE_RIGHT)) {
        this->current_state = RUNNING;

        if (!player.SpriteDirection.test(1)) {
          player.SpriteDirection.flip();
          auto sprite = m_registry.get<stella::components::SpriteComponent>(entity);
          sprite.Sprite->SetScale(glm::vec2(1.0,1.0));
        }
      }

      if (this->Display.IsKeyDown(SDL_SCANCODE_UP)) {
      }

      //auto &anims = m_registry.get<stella::components::AnimationsComponent>(entity);
      //this->SetState(this->current_state, anims, previous_state);
    }

  private:
    void SetState(Player::State state, stella::components::AnimationsComponent &anims, Player::State previous_state)
    {
      if (previous_state != this->current_state && previous_state == RUNNING)
      {
        
      }

      switch(state)
      {
        case RUNNING:
          //anims.current_animation = "run";
          break;
        case JUMPING:
          //anims.current_animation = "jump";
          break;
        case FALLING:
          //anims.current_animation = "fall";
          break;
        case IDLE:
          //anims.current_animation = "idle";
          break;
        default:
          break;
      }
    }
};
