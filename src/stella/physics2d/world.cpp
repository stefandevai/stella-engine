#include "stella/physics2d/world.h"
#include "stella/physics2d/body.h"
#include "stella/physics2d/manifold.h"

#include <algorithm>
#include <cmath>
#include <iostream>

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
    for (auto body2 = body1 + 1; body2 != this->Bodies.end(); ++body2) {
      if (this->AABBvsAABB(*body1, *body2)) {
      }
    }
  }
}

bool World::AABBvsAABB(std::shared_ptr<stella::physics2d::Body> ba, std::shared_ptr<stella::physics2d::Body> bb) {
  // Separating Axis Theorem (SAT) for both bodies
  if (ba->Position.x + ba->Dimension.x <= bb->Position.x || ba->Position.x >= bb->Position.x + bb->Dimension.x) return false;
  if (ba->Position.y + ba->Dimension.y <= bb->Position.y || ba->Position.y >= bb->Position.y + bb->Dimension.y) return false;

  // Resolve collisions if at least one of the bodies isn't static
  if (!ba->IsStatic || !bb->IsStatic) {
    // Gets:
    //    - Vector from A to B
    //    - The amount overlapping in both x and y axis
    glm::vec2 vab = (bb->Position + bb->Dimension*0.5f) - (ba->Position + ba->Dimension*0.5f);
    float overlapx = ba->Dimension.x*0.5f + bb->Dimension.x*0.5f - fabs(vab.x);
    float overlapy = ba->Dimension.y*0.5f + bb->Dimension.y*0.5f - fabs(vab.y);

    if (!ba->IsStatic && bb->IsStatic) {
      stella::physics2d::Manifold mf(ba, bb, glm::vec2(overlapx, overlapy));
      this->ResolveManifold(mf);
    }
    else if (!bb->IsStatic && ba->IsStatic) {
      stella::physics2d::Manifold mf(bb, ba, glm::vec2(overlapx, overlapy));
      this->ResolveManifold(mf);
    }
    else {
      // TODO: Resolve collisions when both bodies are dynamic
    }
  }
  else {
    // TODO: Resolve collisions for static bodies
  }

  // There was a collision
  return true;
}

void World::ResolveManifold(stella::physics2d::Manifold& mf) {
    // Shortcuts for:
    //    - Last *A* bottom
    //    - Last *A* top
    //    - *A* current bottom
    //    - *A* current top
    float last_maxy_a = mf.A->LastPosition.y + mf.A->Dimension.y;
    float last_miny_a = mf.A->LastPosition.y;
    float maxy_a = mf.A->Position.y + mf.A->Dimension.y;
    float miny_a = mf.A->Position.y;

    // Shortcuts for:
    //    - Last *A* right
    //    - Last *A* left
    //    - *A* current right
    //    - *A* current left
    float last_maxx_a = mf.A->LastPosition.x + mf.A->Dimension.x;
    float last_minx_a = mf.A->LastPosition.x;
    float maxx_a = mf.A->Position.x + mf.A->Dimension.x;
    float minx_a = mf.A->Position.x;

    // Shortcuts for:
    //    - *B* bottom
    //    - *B* top
    //    - *B* center y
    float maxy_b = mf.B->Position.y + mf.B->Dimension.y;
    float miny_b = mf.B->Position.y;
    float cyb = mf.B->Position.y + 0.5f*mf.B->Dimension.y;

    // Shortcuts for:
    //    - *B* right
    //    - *B* left
    //    - *B* center x
    float maxx_b = mf.B->Position.x + mf.B->Dimension.x;
    float minx_b = mf.B->Position.x;
    float cxb = mf.B->Position.x + 0.5f*mf.B->Dimension.x;

    // Checks in which axis there was intersection in the last position
    bool last_intersected_x = false, last_intersected_y = false;
    if (last_maxx_a > minx_b && last_minx_a < maxx_b) last_intersected_x = true;
    if (last_maxy_a > miny_b && last_miny_a < maxy_b) last_intersected_y = true;

    // Checks:
    //    - if *A* last bottom is less than the center y of *B*
    //    - if last *A* bottom is greater than the top of *B*
    if (last_maxy_a < cyb && maxy_a >= miny_b && ((last_intersected_x && !last_intersected_y) || (!last_intersected_x && !last_intersected_y))) {
        mf.A->Collisions.set(2);
        mf.A->Acceleration.y = 0.f;
        mf.A->Velocity.y = 0.f;
        mf.A->Position.y -= mf.Penetration.y;
    }

    // Checks:
    //    - if *A* last top is greater than the center y of *B*
    //    - if last *A* top is less than the bottom of *B*
    else if (last_miny_a > cyb && miny_a <= maxy_b && ((last_intersected_x && !last_intersected_y) || (!last_intersected_x && !last_intersected_y))) {
        mf.A->Collisions.set(0);
        mf.A->Acceleration.y = 0.f;
        mf.A->Velocity.y = 0.f;
        mf.A->Position.y += mf.Penetration.y;
    }

    // Checks:
    //    - if *A* last right is less than the center x of *B*
    //    - if last *A* right is greater than the left of *B*
    else if (last_maxx_a < cxb && maxx_a >= minx_b && ((!last_intersected_x && last_intersected_y) || (!last_intersected_x && !last_intersected_y))) {
        mf.A->Collisions.set(1);
        mf.A->Acceleration.x = 0.f;
        mf.A->Velocity.x = 0.f;
        mf.A->Position.x -= mf.Penetration.x;
    }

    // Checks:
    //    - if *A* last left is greater than the center x of *B*
    //    - if last *A* left is less than the right of *B*
    else if (last_minx_a > cxb && minx_a < maxx_b && ((!last_intersected_x && last_intersected_y) || (!last_intersected_x && !last_intersected_y))) {
        mf.A->Collisions.set(3);
        mf.A->Acceleration.x = 0.f;
        mf.A->Velocity.x = 0.f;
        mf.A->Position.x += mf.Penetration.x;
    }
    // Shouldn't get to this point. Left for debug purposes.
    else {
      std::cout << "Shouldn't be here! - Resolve Manifold\n";
    }
}

void World::UpdateMovement(float dt) const {
  for (auto& body: this->Bodies) {
    if (!body->IsStatic) {
      body->LastPosition = body->Position;
      // X movement
      if (fabs(body->Acceleration.x) > 0.f) {
        if (fabs(body->Velocity.x + body->Acceleration.x*dt) <= body->TargetVelocity.x) {
          body->Velocity.x += body->Acceleration.x*dt;
        }
        else {
          body->Velocity.x = body->TargetVelocity.x*fabs(body->Acceleration.x)/body->Acceleration.x;
        }
      }
      else {
        if (fabs(body->Velocity.x) - body->Drag.x*dt > 0.f) {
          //std::cout << "here\n";
          body->Velocity.x -= body->Drag.x*dt*body->Velocity.x/fabs(body->Velocity.x);
          //std::cout << body->Drag.x << std::endl;
          //std::cout << body->Velocity.x << " " << body->Drag.x*body->Velocity.x/fabs(body->Velocity.x) << std::endl;
        }
        else body->Velocity.x = 0.f;
      }
      body->Position.x += body->Velocity.x*dt;

      // Y movement
      if (body->Gravity) body->Acceleration.y += this->Gravity*dt;

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

    if (body->CollideWithBorders)
    {
      if (body->Position.x < 0.f + this->CameraOffset.x)
      {
        body->Position.x = 0.f + this->CameraOffset.x;
      }
      else if (body->Position.x + body->Dimension.x > this->CameraOffset.x + this->ScreenWidth)
      {
        body->Position.x = this->CameraOffset.x + this->ScreenWidth - body->Dimension.x;
      }

      if (body->Position.y < 0.f + this->CameraOffset.y)
      {
        body->Position.y = 0.f + this->CameraOffset.y;
      }
      else if (body->Position.y + body->Dimension.y > this->CameraOffset.y + this->ScreenHeight)
      {
        body->Position.y = this->CameraOffset.y + this->ScreenHeight - body->Dimension.y;
      }
    }
  }
}
}}

