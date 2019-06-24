#pragma once

#include <vector>
#include <stella/physics2d/body.h>

namespace stella {
namespace components {
struct Body2DComponent {
  explicit Body2DComponent(const std::vector<double>& drag = {0.f, 0.f}) : Drag(drag) {}
  ~Body2DComponent() {}

  std::shared_ptr<stella::physics2d::Body> Body;
  bool Initialized = false;
  std::vector<double> Drag;
};
} // namespace components
} // namespace stella

