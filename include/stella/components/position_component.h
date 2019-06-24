#pragma once

namespace stella {
namespace components {
struct PositionComponent {
  PositionComponent(float x, float y)
      : x(x), y(y) {}
  float x, y;
};
} // namespace components
} // namespace stella

