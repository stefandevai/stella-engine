#pragma once

#include <glm/glm.hpp>
#include "stella/physics2d/body.h"

namespace stella {
namespace physics2d {
struct Manifold {
  Manifold(std::shared_ptr<stella::physics2d::Body> a, std::shared_ptr<stella::physics2d::Body> b, glm::vec2 penetration)
    : A(a), B(b), Penetration(penetration) {}
  ~Manifold() {}

  std::shared_ptr<stella::physics2d::Body> A;
  std::shared_ptr<stella::physics2d::Body> B;
  glm::vec2 Penetration;
  glm::vec2 Normal;
};
}}
