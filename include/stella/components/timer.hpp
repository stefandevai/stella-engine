#pragma once

#include "component.hpp"

#include <string>
#include <vector>
#include <functional>

#include <entt/entity/registry.hpp> // IWYU pragma: export

namespace stella
{
namespace component
{
  struct Timer : public Component
  {
    enum Type
    {
      INCREASE,
      DECREASE
    };

    Timer() : Component ("Timer") {}

    // Timer (TimerEvent event, const int duration) : event (event), duration (duration) {}
    Timer (const double duration,
           const double target,
           const Type timer_type,
           std::function<void (entt::registry&, const entt::entity)> fn,
           bool active = true)
      : Component ("Timer"), duration (duration), target (target), timer_type (timer_type), fn (fn), active (active)
    {
    }

    double duration = 0.0;
    double target = 0.0;
    Type timer_type = INCREASE;
    std::function<void (entt::registry&, const entt::entity)> fn;
    bool active = true;
  };

} // namespace component
} // namespace stella
