#include "game_systems.h"

#include <cmath>

#include "../components/game_components.h"

MovementSystem::MovementSystem() {}

MovementSystem::~MovementSystem() {}

void MovementSystem::update(entityx::EntityManager &es,
                            entityx::EventManager &events,
                            entityx::TimeDelta dt) {
  es.each<PositionComponent, MovementComponent>([dt](entityx::Entity entity,
                                                    PositionComponent &pos,
                                                    MovementComponent &mov) {
      if (fabs(mov.Acceleration.x) > 0.000001) {
        if (fabs(mov.Velocity.x) < mov.TargetVelocity.x) {
          mov.Velocity.x += mov.Acceleration.x*dt;
        }
        else {
          mov.Velocity.x = mov.TargetVelocity.x*(mov.Velocity.x/fabs(mov.Velocity.x));
        }
      }
      else {
        if (fabs(mov.Velocity.x) - mov.Drag.x*dt > 0.f) {
          if (mov.Velocity.x > 0.0f) {
            mov.Velocity.x -= mov.Drag.x*dt; 
          }
          else {
            mov.Velocity.x += mov.Drag.x*dt; 
          }
        }
        else mov.Velocity.x = 0.f;
      }
      pos.x += mov.Velocity.x*dt;
  });
}
