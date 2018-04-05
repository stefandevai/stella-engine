#pragma once

#include <vector>
#include <memory>

#include "stella/physics2d/body.h"

namespace stella {
namespace physics2d {
class World {
  public:
    World();
    ~World();
    void AddBody(std::shared_ptr<stella::physics2d::Body> body);
    void RemoveBody(std::shared_ptr<stella::physics2d::Body> body);
    void Update(float dt);

  private:
    static constexpr float GRAVITY = 1000.f;
    std::vector<std::shared_ptr<stella::physics2d::Body>> Bodies;

    void UpdateCollisions(float dt);
    void UpdateMovement(float dt);
};
}}

