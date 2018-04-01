#include "player_movement_system.h"

#include <SDL2/SDL.h>

#include "../components/game_components.h"

PlayerMovementSystem::PlayerMovementSystem(const int &boundx, stella::graphics::Display &display)
    : BoundX(boundx), Display(display) {}

PlayerMovementSystem::~PlayerMovementSystem() {}

void PlayerMovementSystem::update(entityx::EntityManager &es,
                                  entityx::EventManager &events,
                                  entityx::TimeDelta dt) {
  es.each<MovementComponent, PlayerComponent>(
      [this, &dt](entityx::Entity entity, MovementComponent &mov, PlayerComponent &player) {

        mov.Acceleration.x = 0.f;
        if (this->Display.IsKeyDown(SDL_SCANCODE_LEFT)) {
          mov.Acceleration.x -= 400.f;
        }
        if (this->Display.IsKeyDown(SDL_SCANCODE_RIGHT)) {
          mov.Acceleration.x += 400.f;
        }

				//if(this->Display.IsKeyDown(SDL_SCANCODE_LEFT) || this->Display.IsKeyDown(SDL_SCANCODE_RIGHT)) {
					//float final_acceleration = 0.f;
          //if (this->Display.IsKeyDown(SDL_SCANCODE_LEFT)) {
            //final_acceleration -= 400.f;
          //}
          //if (this->Display.IsKeyDown(SDL_SCANCODE_RIGHT)) {
            //final_acceleration += 400.f;
          //}
          //mov.Acceleration.x = final_acceleration;
				//}
        //else mov.Acceleration.x = 0.f;

      });
}
