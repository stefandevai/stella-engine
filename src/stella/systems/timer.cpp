#include "stella/systems/timer.hpp"
#include "stella/components/timer.hpp"

namespace stella
{
namespace system
{
  Timer::Timer (entt::registry& registry) {}

  void Timer::update (entt::registry& registry, const double dt)
  {
    registry.view<component::Timer>().each ([&registry, dt] (auto entity, auto& timer) {
      if (timer.active)
      {
        switch (timer.timer_type)
        {
          case component::Timer::Type::INCREASE:
            timer.duration += dt * 1000;

            if (timer.duration >= timer.target)
            {
              timer.active = false;
              timer.fn (registry, entity);
            }
            break;

          case component::Timer::Type::DECREASE:
            timer.duration -= dt * 1000.0;

            if (timer.duration <= timer.target)
            {
              timer.active = false;
              timer.fn (registry, entity);
            }
            break;

          default:
            break;
        }
      }
    });
  }
} // namespace system
} // namespace stella
