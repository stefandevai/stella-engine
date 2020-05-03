#pragma once

#include "system.hpp"

namespace stella
{
namespace system
{
  class AnimationPlayer : public System
  {
  public:
    AnimationPlayer ();
    void update (entt::registry& registry, const double dt) override;
  };
} // namespace system
} // namespace stella
