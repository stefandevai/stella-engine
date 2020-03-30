#pragma once

#include "stella/components/character_animation.hpp"
#include "stella/components/animation.hpp"
#include "stella/components/body2d.hpp"
#include "system.hpp"

namespace stella
{
namespace system
{
  class CharacterAnimation : public System
  {
  public:
    CharacterAnimation();
    void update (entt::registry& registry, const double dt) override;
  };
} // namespace system
} // namespace stella
