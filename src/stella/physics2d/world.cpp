#include "stella/physics2d/world.h"

#include <algorithm>

namespace stella {
namespace physics2d {

World::World() {

}

World::~World() {
  this->Bodies.clear();
}

void World::AddBody(std::shared_ptr<stella::physics2d::Body> body) {
  this->Bodies.emplace_back(body);
}

void World::RemoveBody(std::shared_ptr<stella::physics2d::Body> body) {
  auto it = std::find(this->Bodies.begin(), this->Bodies.end(), body);
  if (it != this->Bodies.end())
    this->Bodies.erase(it);
}

void World::Update(float dt) {
  this->UpdateCollisions(dt);
  this->UpdateMovement(dt);
}

void World::UpdateCollisions(float dt) {

}

void World::UpdateMovement(float dt) {
  for (auto& body: this->Bodies) {
    // X movement
    if (fabs(body->Acceleration.x) > 0.f) {
      if (fabs(body->Velocity.x) <= body->TargetVelocity.x) {
        body->Velocity.x += body->Acceleration.x*dt;
      }
      else {
        body->Velocity.x = body->TargetVelocity.x*fabs(body->Velocity.x)/body->Velocity.x;
      }
    }
    else {
      if (fabs(body->Velocity.x) - body->Drag.x*dt > 0.f) {
        body->Velocity.x -= body->Drag.x*dt*body->Velocity.x/fabs(body->Velocity.x);
      }
      else body->Velocity.x = 0.f;
    }
    body->Position.x += body->Velocity.x*dt;

    // Y movement
    if (body->Gravity) body->Acceleration.y += GRAVITY*dt;

    if (fabs(body->Acceleration.y) > 0.000001) {
      if (fabs(body->Velocity.y) < body->TargetVelocity.y) {
        body->Velocity.y += body->Acceleration.y*dt;
      }
      else {
        body->Velocity.y = body->TargetVelocity.y*(body->Acceleration.y/fabs(body->Acceleration.y));
      }
    }
    else {
      if (fabs(body->Velocity.y) - body->Drag.y*dt > 0.f) {
        body->Velocity.y -= body->Drag.y*dt*body->Velocity.y/fabs(body->Velocity.y);
      }
      else body->Velocity.y = 0.f;
    }
    body->Position.y += body->Velocity.y*dt;
  }

}

}}

