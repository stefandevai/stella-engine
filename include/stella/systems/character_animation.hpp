#pragma once

#include "entt/entity/fwd.hpp"
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
