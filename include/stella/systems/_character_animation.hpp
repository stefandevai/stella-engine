#pragma once

#include "system.hpp"

namespace stella
{
namespace system
{
  class CharacterAnimation : public System
  {
  public:
    CharacterAnimation();
    void update (entt::registry& registry, const double dt);
  };
} // namespace system
} // namespace stella
