#pragma once

#include "system.hpp"

namespace stella
{
namespace system
{
  class AnimationPlayer : public System
  {
  public:
    AnimationPlayer();
    void update (entt::registry& registry, const double dt);
  };
} // namespace system
} // namespace stella
