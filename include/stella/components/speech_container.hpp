#pragma once

#include <entt/entity/registry.hpp>
#include <vector>

namespace stella
{
namespace component
{
  struct SpeechContainer
  {
    SpeechContainer() {}
    std::vector<entt::entity> messages;
  };
} // namespace component
} // namespace stella
