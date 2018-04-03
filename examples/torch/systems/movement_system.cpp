#include "game_systems.h"

#include <cmath>

#include "../components/game_components.h"

const static float GRAVITY = 1000.f;

MovementSystem::MovementSystem() {}

MovementSystem::~MovementSystem() {}

void MovementSystem::update(ex::EntityManager &es,
                            ex::EventManager &events,
                            ex::TimeDelta dt) {
  es.each<PositionComponent, MovementComponent>([dt](ex::Entity entity,
                                                    PositionComponent &pos,
                                                    MovementComponent &mov) {
      // X movement
      if (fabs(mov.Acceleration.x) > 0.f) {
        if (fabs(mov.Velocity.x) <= mov.TargetVelocity.x) {
          mov.Velocity.x += mov.Acceleration.x*dt;
        }
        else {
          mov.Velocity.x = mov.TargetVelocity.x*fabs(mov.Velocity.x)/mov.Velocity.x;
        }
      }
      else {
        if (fabs(mov.Velocity.x) - mov.Drag.x*dt > 0.f) {
          mov.Velocity.x -= mov.Drag.x*dt*mov.Velocity.x/fabs(mov.Velocity.x);
        }
        else mov.Velocity.x = 0.f;
      }
      pos.x += mov.Velocity.x*dt;

      // Y movement
      if (mov.Gravity) mov.Acceleration.y += GRAVITY*dt;

      if (fabs(mov.Acceleration.y) > 0.000001) {
        if (fabs(mov.Velocity.y) < mov.TargetVelocity.y) {
          mov.Velocity.y += mov.Acceleration.y*dt;
        }
        else {
          mov.Velocity.y = mov.TargetVelocity.y*(mov.Acceleration.y/fabs(mov.Acceleration.y));
        }
      }
      else {
        if (fabs(mov.Velocity.y) - mov.Drag.y*dt > 0.f) {
          mov.Velocity.y -= mov.Drag.y*dt*mov.Velocity.y/fabs(mov.Velocity.y);
        }
        else mov.Velocity.y = 0.f;
      }
      pos.y += mov.Velocity.y*dt;
  });
}
