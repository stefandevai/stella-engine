#pragma once

#include <stella/components.h>
#include <SDL2/SDL.h>

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
    enum State {IDLE_U, IDLE_D, IDLE_H};
    const entt::registry::entity_type entity = m_registry.create();
  private:
    stella::graphics::Display &Display;
    State current_state = IDLE_D;

  public:
    Player(entt::registry &registry, stella::graphics::Display &display) 
      : m_registry(registry), Display(display)
    { 
      
    }

    ~Player() { }

    void update()
    {
      auto &player = m_registry.get<stella::components::PlayerComponent>(entity);
      auto &body = m_registry.get<stella::components::Body2DComponent>(entity);
      auto previous_state = this->current_state;

      // Handle input
      if (this->Display.IsKeyDown(SDL_SCANCODE_LEFT)) 
      {
        body.Body->MoveLeft();
        this->current_state = IDLE_H;

        if (!player.SpriteDirection.test(0)) {
          player.SpriteDirection.flip();
          auto sprite = m_registry.get<stella::components::SpriteComponent>(entity);
          sprite.Sprite->SetScale(glm::vec2(-1.0,1.0));
        }
      }

      if (this->Display.IsKeyDown(SDL_SCANCODE_RIGHT))
      {
        body.Body->MoveRight();
        this->current_state = IDLE_H;

        if (!player.SpriteDirection.test(1)) {
          player.SpriteDirection.flip();
          auto sprite = m_registry.get<stella::components::SpriteComponent>(entity);
          sprite.Sprite->SetScale(glm::vec2(1.0,1.0));
        }
      }

      if (this->Display.IsKeyDown(SDL_SCANCODE_UP))
      {
        body.Body->MoveTop();
        this->current_state = IDLE_U;
      }

      if (this->Display.IsKeyDown(SDL_SCANCODE_DOWN))
      {
        body.Body->MoveBottom();
        this->current_state = IDLE_D;
      }

      auto &anims = m_registry.get<stella::components::AnimationsComponent>(entity);
      this->SetState(this->current_state, anims, previous_state);
    }

  private:
    void SetState(Player::State state, stella::components::AnimationsComponent &anims, Player::State previous_state)
    {
      //if (previous_state != this->current_state && previous_state == IDLE_)
      //{
        
      //}

      switch(state)
      {
        case IDLE_U:
          anims.current_animation = "idle-u";
          break;
        case IDLE_D:
          anims.current_animation = "idle-d";
          break;
        case IDLE_H:
          anims.current_animation = "idle-h";
          break;
        default:
          break;
      }
    }
};
