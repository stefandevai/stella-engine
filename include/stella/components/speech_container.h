#pragma once

#include <vector>
#include <entt/entity/registry.hpp>

namespace stella {
namespace components {
struct SpeechContainer {
  SpeechContainer() {}
  std::vector<entt::entity> messages;
};
} // namespace components
} // namespace stella

