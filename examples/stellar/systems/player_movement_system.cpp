#include "player_movement_system.h"

#include <SDL2/SDL.h>

#include "../components/body_component.h"
#include "../components/input_component.h"
#include "../components/movement_component.h"
#include "../components/spatial_component.h"

PlayerMovementSystem::PlayerMovementSystem(const int &boundx, stella::graphics::Display &display)
    : BoundX(boundx), Display(display) {}

PlayerMovementSystem::~PlayerMovementSystem() {}

void PlayerMovementSystem::update(entityx::EntityManager &es,
                                  entityx::EventManager &events,
                                  entityx::TimeDelta dt) {
  es.each<MovementComponent, BodyComponent, SpatialComponent, InputComponent>(
      [this](entityx::Entity entity, MovementComponent &mov,
             BodyComponent &body, SpatialComponent &spa,
             InputComponent &input) {
        float jumpforce = 3.5f;

        // Horizontal movement
				//if (this->Display.IsKeyDown(SDL_SCANCODE_LEFT) && !this->Display.IsKeyDown(SDL_SCANCODE_RIGHT)) {
				if (this->Display.IsKeyDown(SDL_SCANCODE_LEFT)) {
          mov.accelX(-1.0f);
				}
				if (this->Display.IsKeyDown(SDL_SCANCODE_RIGHT)) {
          mov.accelX(1.0f);
				}
				//else {
					//mov.desaccelX();
				//}

        // Jump only if body is colliding bottom
        if (body.ColDir.test(0)) {
          mov.stopY();
          if (this->Display.IsKeyDown(SDL_SCANCODE_UP)) {
            mov.Vel.y = -jumpforce;
          }
        } else {
          mov.Gravity = true;
        }

        if (spa.x < 0) {
          spa.x = 0;
          mov.stopX();
        } else if (spa.x + spa.w > this->BoundX) {
          spa.x = this->BoundX - spa.w;
          // mov.stopX();
        }
      });
}
