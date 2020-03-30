#pragma once

#include "stella/components/character_animation.h"
#include "stella/components/animation.h"
#include "stella/components/body2d.h"
#include "system.h"

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
