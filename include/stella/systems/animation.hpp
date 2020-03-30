#pragma once

#include "system.hpp"
#include "stella/components/animation.hpp"

namespace stella
{
namespace system
{
  class Animation : public System
  {
  public:
    Animation (entt::registry& registry);
    void update (entt::registry& registry, const double dt) override;
    void initialize_animation (entt::registry& registry, entt::entity entity, component::Animation& animations);
  };
} // namespace system
} // namespace stella
