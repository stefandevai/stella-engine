#include "player_movement_system.h"

#include "../components/body_component.h"
#include "../components/input_component.h"
#include "../components/movement_component.h"
#include "../components/spatial_component.h"
#include "../components/sprite_component.h"

#include <GLFW/glfw3.h>

PlayerMovementSystem::PlayerMovementSystem(const int &boundx, stella::graphics::Display &display)
    : BoundX(boundx), Display(display) {}

PlayerMovementSystem::~PlayerMovementSystem() {}

void PlayerMovementSystem::update(entityx::EntityManager &es,
                                  entityx::EventManager &events,
                                  entityx::TimeDelta dt) {
  es.each<MovementComponent, InputComponent,
          TextureComponent>([this](entityx::Entity entity,
                                   MovementComponent &mov,
                                   InputComponent &input,
                                   TextureComponent &tex) {
    if (this->Display.IsKeyDown(GLFW_KEY_UP)) {
      mov.NewDirection = 0;
      // tex.sprite->SetDirectFrame(0);
    } else if (this->Display.IsKeyDown(GLFW_KEY_RIGHT)) {
      mov.NewDirection = 1;
      // tex.sprite->SetDirectFrame(1);
    } else if (this->Display.IsKeyDown(GLFW_KEY_DOWN)) {
      mov.NewDirection = 2;
      // tex.sprite->SetDirectFrame(2);
    } else if (this->Display.IsKeyDown(GLFW_KEY_LEFT)) {
      mov.NewDirection = 3;
      // tex.sprite->SetDirectFrame(3);
    }
  });
}
