#include "stella/physics2d/world.h"

#include <iostream>
#include <algorithm>
#include <cmath>

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
  this->UpdateMovement(dt);
  this->UpdateCollisions();
}

void World::UpdateCollisions() {
  for (auto body: this->Bodies)
    body->Collisions.reset();

  for (auto body1 = this->Bodies.begin(); body1 != this->Bodies.end(); ++body1) {
    bool collided = false;
    for (auto body2 = body1 + 1; body2 != this->Bodies.end(); ++body2) {
      if (this->AABBvsAABB(*body1, *body2)) {
        collided = true;
      }
    }
  }
}

bool World::AABBvsAABB(std::shared_ptr<stella::physics2d::Body> ba, std::shared_ptr<stella::physics2d::Body> bb) {
  if (ba->Position.x + ba->Dimension.x <= bb->Position.x || ba->Position.x >= bb->Position.x + bb->Dimension.x) return false;
  if (ba->Position.y + ba->Dimension.y <= bb->Position.y || ba->Position.y >= bb->Position.y + bb->Dimension.y) return false;

  glm::vec2 vab = (bb->Position + bb->Dimension*0.5f) - (ba->Position + ba->Dimension*0.5f);

  float overlapx = ba->Dimension.x*0.5f + bb->Dimension.x*0.5f - fabs(vab.x);
  float overlapy = ba->Dimension.y*0.5f + bb->Dimension.y*0.5f - fabs(vab.y);

  stella::physics2d::Manifold mf(ba, bb, glm::vec2(overlapx, overlapy));
  this->ResolveManifold(mf);
  return true;
}

void World::ResolveManifold(stella::physics2d::Manifold& mf) {
  if (!mf.A->IsStatic) {
    float position_projection_bottom = mf.A->LastPosition.y + mf.A->Dimension.y + mf.A->Velocity.y;
    float position_projection_top = mf.A->LastPosition.y + mf.A->Velocity.y;

    float position_projection_right = mf.A->LastPosition.x + mf.A->Dimension.x + mf.A->Velocity.x;
    float position_projection_left = mf.A->LastPosition.x + mf.A->Velocity.x;

    float cbx = mf.B->Position.x + mf.B->Dimension.x/2.f;
    float cby = mf.B->Position.y + mf.B->Dimension.y/2.f;

    float cax = mf.A->Position.x + mf.A->Dimension.x/2.f;
    float cay = mf.A->Position.y + mf.A->Dimension.y/2.f;

    if (mf.Penetration.y <= mf.Penetration.x) {
      if (position_projection_bottom >= mf.B->Position.y && cay < cby) {
        mf.A->Collisions.set(2);
        mf.A->Acceleration.y = 0.f;
        mf.A->Velocity.y = 0.f;
        mf.A->Position.y -= mf.Penetration.y;
        //std::cout << "Colliding bottom\n";
      }
      else if (position_projection_top <= mf.B->Position.y + mf.B->Dimension.y && cay > cby) {
        mf.A->Collisions.set(0);
        mf.A->Acceleration.y = 0.f;
        mf.A->Velocity.y = 0.f;
        mf.A->Position.y += mf.Penetration.y;
        //std::cout << "Colliding top\n";
      }
      else {
        std::cout << "Shouldn't be here!! Resolve manifold - top\n";
      }
    }
    else {
      if (position_projection_right >= mf.B->Position.x && cax < cbx) {
        mf.A->Collisions.set(1);
        mf.A->Acceleration.x = 0.f;
        mf.A->Velocity.x = 0.f;
        mf.A->Position.x -= mf.Penetration.x;
        //std::cout << "Colliding right\n";
      }
      else if (position_projection_left <= mf.B->Position.x + mf.B->Dimension.x && cax > cbx) {
        mf.A->Collisions.set(3);
        mf.A->Acceleration.x = 0.f;
        mf.A->Velocity.x = 0.f;
        mf.A->Position.x += mf.Penetration.x;
        //std::cout << "Colliding left\n";
      }
      else {
        std::cout << "Shouldn't be here!! Resolve manifold - left\n";
      }
    }
  }
}

void World::UpdateMovement(float dt) {
  for (auto& body: this->Bodies) {
    if (!body->IsStatic) {
      body->LastPosition = body->Position;
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

      if (fabs(body->Acceleration.y) > 0.f) {
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
}

}}

