#pragma once

#include <vector>
#include <memory>

#include "stella/physics2d/body.h"
#include "stella/physics2d/manifold.h"

namespace stella {
namespace physics2d {
class World {
  public:
    World();
    ~World();
    void AddBody(std::shared_ptr<stella::physics2d::Body> body);
    void RemoveBody(std::shared_ptr<stella::physics2d::Body> body);
    void Update(float dt);
    void SetGravity(float new_gravity) { this->Gravity = new_gravity; }

  private:
    float Gravity = 2000.f;
    std::vector<std::shared_ptr<stella::physics2d::Body>> Bodies;
    std::vector<std::shared_ptr<stella::physics2d::Manifold>> Manifolds;

    void UpdateMovement(float dt);
    void UpdateCollisions();

    bool AABBvsAABB(std::shared_ptr<stella::physics2d::Body> ba, std::shared_ptr<stella::physics2d::Body> bb);
    void ResolveManifolds();
    void ResolveManifold(stella::physics2d::Manifold& mf);
};
}}

