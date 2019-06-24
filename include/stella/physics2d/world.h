#pragma once

#include <vector>
#include <memory>

namespace stella {
namespace physics2d {
class Manifold;
class Body;

class World {
  public:
    World();
    ~World();
    void AddBody(std::shared_ptr<stella::physics2d::Body> body);
    void RemoveBody(std::shared_ptr<stella::physics2d::Body> body);
    void Update(float dt);
    void SetGravity(float new_gravity) { this->Gravity = new_gravity; }

  private:
    float Gravity = 1200.f;
    std::vector<std::shared_ptr<stella::physics2d::Body>> Bodies;
    std::vector<std::shared_ptr<stella::physics2d::Manifold>> Manifolds;

    void UpdateMovement(float dt) const;
    void UpdateCollisions();

    bool AABBvsAABB(std::shared_ptr<stella::physics2d::Body> ba, std::shared_ptr<stella::physics2d::Body> bb);
    void ResolveManifolds();
    static void ResolveManifold(stella::physics2d::Manifold& mf);
};
}}

