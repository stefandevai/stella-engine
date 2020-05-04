#pragma once

#include "component.hpp"

#include <entt/entity/registry.hpp>
#include <vector>

namespace stella
{
namespace component
{
  struct SpeechContainer : public Component
  {
    SpeechContainer() : Component ("Speech Container") {}
    std::vector<entt::entity> messages;
  };
} // namespace component
} // namespace stella
