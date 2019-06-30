#pragma once

#include <vector>
#include <stella/physics2d/body.h>

namespace stella {
namespace components {
struct Body2DComponent {
  Body2DComponent(const std::vector<double>& drag = {0.f, 0.f}, bool collide_with_borders = false) : Drag(drag), CollideWithBorders(collide_with_borders) {}
  ~Body2DComponent() {}

  std::shared_ptr<stella::physics2d::Body> Body;
  bool Initialized = false;
  std::vector<double> Drag;
  bool CollideWithBorders;
};
} // namespace components
} // namespace stella

