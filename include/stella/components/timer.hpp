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

    // Timer (TimerEvent event, const int duration) : event (event), duration (duration) {}
    Timer (const double duration,
           const double target,
           const Type timer_type,
           std::function<void (entt::registry&, const entt::entity)> fn,
           bool active = true)
      : Component ("Timer"), duration (duration), target (target), timer_type (timer_type), fn (fn), active (active)
    {
    }

    double duration;
    double target;
    Type timer_type;
    std::function<void (entt::registry&, const entt::entity)> fn;
    bool active;
  };

} // namespace component
} // namespace stella
