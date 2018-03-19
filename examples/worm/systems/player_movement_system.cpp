#include "player_movement_system.h"

#include "../components/body_component.h"
#include "../components/input_component.h"
#include "../components/movement_component.h"
#include "../components/spatial_component.h"
#include "../components/sprite_component.h"

#include <GLFW/glfw3.h>

PlayerMovementSystem::PlayerMovementSystem(const int &boundx)
    : BoundX(boundx) {}

PlayerMovementSystem::~PlayerMovementSystem() {}

void PlayerMovementSystem::update(entityx::EntityManager &es,
                                  entityx::EventManager &events,
                                  entityx::TimeDelta dt) {
  es.each<MovementComponent, SpatialComponent, InputComponent,
          TextureComponent>([this](entityx::Entity entity,
                                   MovementComponent &mov,
                                   SpatialComponent &spa, InputComponent &input,
                                   TextureComponent &tex) {
    if (input.Keys[GLFW_KEY_UP]) {
      mov.NewDirection = 0;
      // tex.sprite->SetDirectFrame(0);
    } else if (input.Keys[GLFW_KEY_RIGHT]) {
      mov.NewDirection = 1;
      // tex.sprite->SetDirectFrame(1);
    } else if (input.Keys[GLFW_KEY_DOWN]) {
      mov.NewDirection = 2;
      // tex.sprite->SetDirectFrame(2);
    } else if (input.Keys[GLFW_KEY_LEFT]) {
      mov.NewDirection = 3;
      // tex.sprite->SetDirectFrame(3);
    }
  });
}
