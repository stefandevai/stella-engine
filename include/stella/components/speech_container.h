#pragma once

#include <entt/entity/registry.hpp>
#include <vector>

namespace stella
{
namespace components
{
  struct SpeechContainer
  {
    SpeechContainer() {}
    std::vector<entt::entity> messages;
  };
} // namespace components
} // namespace stella
