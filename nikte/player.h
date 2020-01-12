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
    enum State {IDLE, RUNNING, JUMPING, FALLING};
    const entt::registry::entity_type entity = m_registry.create();
  private:
    stella::graphics::Display &Display;
    State current_state = IDLE;
    //unsigned m_walk_delay = 0;
    //unsigned WALK_DELAY = 10;

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
      //auto &pos = m_registry.get<stella::components::PositionComponent>(entity);
      auto previous_state = this->current_state;

      // Handle input
      /*if (m_walk_delay > 0)
      {
        --m_walk_delay;
      }*/

      if (this->Display.IsKeyDown(SDL_SCANCODE_LEFT)) {
        if (!body.Body->Moving())
        {
          body.Body->MoveLeft();
        }

        /*if (m_walk_delay == 0)
        {
          pos.x -= 32.f;
          m_walk_delay = WALK_DELAY;
        }*/
      }
      if (this->Display.IsKeyDown(SDL_SCANCODE_RIGHT)) {
        /*if (m_walk_delay == 0)
        {
          pos.x += 32.f;
          m_walk_delay = WALK_DELAY;
        }*/

        if (!body.Body->Moving())
        {
          body.Body->MoveRight();
        }
      }

      if (this->Display.IsKeyDown(SDL_SCANCODE_UP)) {
        if (!body.Body->Moving())
        {
          body.Body->MoveTop();
        }

        /*if (m_walk_delay == 0)
        {
          pos.y -= 32.f;
          m_walk_delay = WALK_DELAY;
        }*/
      }

      if (this->Display.IsKeyDown(SDL_SCANCODE_DOWN)) {
        if (!body.Body->Moving())
        {
          body.Body->MoveBottom();
        }

        /*if (m_walk_delay == 0)
        {
          pos.y += 32.f;
          m_walk_delay = WALK_DELAY;
        }*/
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
