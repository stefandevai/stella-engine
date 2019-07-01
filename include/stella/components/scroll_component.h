#pragma once

#include <glm/glm.hpp>

namespace stella {
namespace components {
struct ScrollComponent {
  ScrollComponent(glm::vec2 speed) : speed(speed) {}
  glm::vec2 speed;
};
} // namespace components
} // namespace stella

