#pragma once

#include <stella/components.h>
#include <SDL2/SDL.h>
#undef main

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
    enum State {IDLE, WALKING};
    const entt::registry::entity_type entity = m_registry.create();
    
  private:
    stella::graphics::Display &Display;
    State current_state = IDLE;
    char direction = 'd'; // Direction to which the player is facing (u, d, l, r) 

  public:
    Player(entt::registry &registry, stella::graphics::Display &display) 
      : m_registry(registry), Display(display)
    { 
      
    }

    ~Player() { }

    void update()
    {
      //auto &player = m_registry.get<stella::components::PlayerComponent>(entity);
      auto &body = m_registry.get<stella::components::Body2DComponent>(entity);
      auto previous_state = this->current_state;

      // Handle input
      this->current_state = IDLE;
      if (this->Display.IsKeyDown(SDL_SCANCODE_LEFT)) 
      {
        body.Body->MoveLeft();
        this->current_state = WALKING;
        this->direction = 'l';
      }

      if (this->Display.IsKeyDown(SDL_SCANCODE_RIGHT))
      {
        body.Body->MoveRight();
        this->current_state = WALKING;
        this->direction = 'r';
      }

      if (this->Display.IsKeyDown(SDL_SCANCODE_UP))
      {
        body.Body->MoveTop();
        this->current_state = WALKING;
        this->direction = 'u';
      }

      if (this->Display.IsKeyDown(SDL_SCANCODE_DOWN))
      {
        body.Body->MoveBottom();
        this->current_state = WALKING;
        this->direction = 'd';
      }

      auto &anims = m_registry.get<stella::components::AnimationsComponent>(entity);
      this->SetState(this->current_state, anims, previous_state);
    }

  private:
    void SetState(Player::State state, stella::components::AnimationsComponent &anims, Player::State previous_state)
    {
      std::string animation_name = "";
      switch(state)
      {
        case IDLE:
          animation_name.append("idle-");
          animation_name.append(1, this->direction);
          anims.current_animation = animation_name;
          break;
        case WALKING:
          animation_name.append("walking-");
          animation_name.append(1, this->direction);
          anims.current_animation = animation_name;
          break;
        default:
          break;
      }
    }
};
